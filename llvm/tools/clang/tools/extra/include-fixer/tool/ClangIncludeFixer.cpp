//===-- ClangIncludeFixer.cpp - Standalone include fixer ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "InMemorySymbolIndex.h"
#include "IncludeFixer.h"
#include "SymbolIndexManager.h"
#include "YamlSymbolIndex.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace llvm;

namespace {
cl::OptionCategory IncludeFixerCategory("Tool options");

enum DatabaseFormatTy {
  fixed, ///< Hard-coded mapping.
  yaml,  ///< Yaml database created by find-all-symbols.
};

cl::opt<DatabaseFormatTy> DatabaseFormat(
    "db", cl::desc("Specify input format"),
    cl::values(clEnumVal(fixed, "Hard-coded mapping"),
               clEnumVal(yaml, "Yaml database created by find-all-symbols"),
               clEnumValEnd),
    cl::init(yaml), cl::cat(IncludeFixerCategory));

cl::opt<std::string> Input("input",
                           cl::desc("String to initialize the database"),
                           cl::cat(IncludeFixerCategory));

cl::opt<bool>
    MinimizeIncludePaths("minimize-paths",
                         cl::desc("Whether to minimize added include paths"),
                         cl::init(true), cl::cat(IncludeFixerCategory));

cl::opt<bool> Quiet("q", cl::desc("Reduce terminal output"), cl::init(false),
                    cl::cat(IncludeFixerCategory));

int includeFixerMain(int argc, const char **argv) {
  tooling::CommonOptionsParser options(argc, argv, IncludeFixerCategory);
  tooling::ClangTool tool(options.getCompilations(),
                          options.getSourcePathList());

  // Set up data source.
  auto SymbolIndexMgr = llvm::make_unique<include_fixer::SymbolIndexManager>();
  switch (DatabaseFormat) {
  case fixed: {
    // Parse input and fill the database with it.
    // <symbol>=<header><, header...>
    // Multiple symbols can be given, separated by semicolons.
    std::map<std::string, std::vector<std::string>> SymbolsMap;
    SmallVector<StringRef, 4> SemicolonSplits;
    StringRef(Input).split(SemicolonSplits, ";");
    std::vector<find_all_symbols::SymbolInfo> Symbols;
    for (StringRef Pair : SemicolonSplits) {
      auto Split = Pair.split('=');
      std::vector<std::string> Headers;
      SmallVector<StringRef, 4> CommaSplits;
      Split.second.split(CommaSplits, ",");
      for (StringRef Header : CommaSplits)
        Symbols.push_back(find_all_symbols::SymbolInfo(
            Split.first.trim(),
            find_all_symbols::SymbolInfo::SymbolKind::Unknown, Header.trim(), 1,
            {}));
    }
    SymbolIndexMgr->addSymbolIndex(
        llvm::make_unique<include_fixer::InMemorySymbolIndex>(Symbols));
    break;
  }
  case yaml: {
    llvm::ErrorOr<std::unique_ptr<include_fixer::YamlSymbolIndex>> DB(nullptr);
    if (!Input.empty()) {
      DB = include_fixer::YamlSymbolIndex::createFromFile(Input);
    } else {
      // If we don't have any input file, look in the directory of the first
      // file and its parents.
      SmallString<128> AbsolutePath(
          tooling::getAbsolutePath(options.getSourcePathList().front()));
      StringRef Directory = llvm::sys::path::parent_path(AbsolutePath);
      DB = include_fixer::YamlSymbolIndex::createFromDirectory(
          Directory, "find_all_symbols_db.yaml");
    }

    if (!DB) {
      llvm::errs() << "Couldn't find YAML db: " << DB.getError().message()
                   << '\n';
      return 1;
    }

    SymbolIndexMgr->addSymbolIndex(std::move(*DB));
    break;
  }
  }

  // Now run our tool.
  std::vector<tooling::Replacement> Replacements;
  include_fixer::IncludeFixerActionFactory Factory(
      *SymbolIndexMgr, Replacements, MinimizeIncludePaths);

  tool.run(&Factory); // Always succeeds.

  if (!Quiet)
    for (const tooling::Replacement &Replacement : Replacements)
      llvm::errs() << "Added " << Replacement.getReplacementText();

  // Set up a new source manager for applying the resulting replacements.
  IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts(new DiagnosticOptions);
  DiagnosticsEngine Diagnostics(new DiagnosticIDs, &*DiagOpts);
  TextDiagnosticPrinter DiagnosticPrinter(outs(), &*DiagOpts);
  SourceManager SM(Diagnostics, tool.getFiles());
  Diagnostics.setClient(&DiagnosticPrinter, false);

  // Write replacements to disk.
  Rewriter Rewrites(SM, LangOptions());
  tooling::applyAllReplacements(Replacements, Rewrites);
  return Rewrites.overwriteChangedFiles();
}

} // namespace

int main(int argc, const char **argv) {
  return includeFixerMain(argc, argv);
}
