//===---------- IncludeInserter.h - clang-tidy ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_INCLUDEINSERTER_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_INCLUDEINSERTER_H

#include "IncludeSorter.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/PPCallbacks.h"
#include <memory>
#include <string>

namespace clang {
namespace tidy {
namespace utils {

// IncludeInserter can be used by ClangTidyChecks in the following fashion:
// class MyCheck : public ClangTidyCheck {
//  public:
//   void registerPPCallbacks(CompilerInstance& Compiler) override {
//     Inserter.reset(new IncludeInserter(&Compiler.getSourceManager(),
//                                        &Compiler.getLangOpts()));
//     Compiler.getPreprocessor().addPPCallbacks(
//         Inserter->CreatePPCallback());
//   }
//
//   void registerMatchers(ast_matchers::MatchFinder* Finder) override { ... }
//
//   void check(const ast_matchers::MatchFinder::MatchResult& Result) override {
//     ...
//     Inserter->CreateIncludeInsertion(
//         Result.SourceManager->getMainFileID(), "path/to/Header.h",
//         /*IsAngled=*/false);
//     ...
//   }
//
//  private:
//   std::unique_ptr<IncludeInserter> Inserter;
// };
class IncludeInserter {
public:
  IncludeInserter(const SourceManager &SourceMgr, const LangOptions &LangOpts,
                  IncludeSorter::IncludeStyle Style);
  ~IncludeInserter();

  // Create PPCallbacks for registration with the compiler's preprocessor.
  std::unique_ptr<PPCallbacks> CreatePPCallbacks();

  // Creates a Header inclusion directive fixit. Returns None on error or
  // if inclusion directive already exists.
  llvm::Optional<FixItHint>
  CreateIncludeInsertion(FileID FileID, llvm::StringRef Header, bool IsAngled);

private:
  void AddInclude(StringRef file_name, bool IsAngled,
                  SourceLocation hash_location, SourceLocation end_location);

  llvm::DenseMap<FileID, std::unique_ptr<IncludeSorter>> IncludeSorterByFile;
  llvm::DenseMap<FileID, std::set<std::string>> InsertedHeaders;
  const SourceManager &SourceMgr;
  const LangOptions &LangOpts;
  const IncludeSorter::IncludeStyle Style;
  friend class IncludeInserterCallback;
};

} // namespace utils
} // namespace tidy
} // namespace clang
#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_INCLUDEINSERTER_H
