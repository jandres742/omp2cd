//===--- FasterStringFindCheck.cpp - clang-tidy----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "FasterStringFindCheck.h"
#include "../utils/OptionsUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/Optional.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace performance {

namespace {

llvm::Optional<std::string> MakeCharacterLiteral(const StringLiteral *Literal) {
  std::string Result;
  {
    llvm::raw_string_ostream OS(Result);
    Literal->outputString(OS);
  }
  // Now replace the " with '.
  auto pos = Result.find_first_of('"');
  if (pos == Result.npos) return llvm::None;
  Result[pos] = '\'';
  pos = Result.find_last_of('"');
  if (pos == Result.npos) return llvm::None;
  Result[pos] = '\'';
  return Result;
}

AST_MATCHER(StringLiteral, lengthIsOne) { return Node.getLength() == 1; }

AST_MATCHER_FUNCTION(ast_matchers::internal::Matcher<Expr>,
                     hasSubstitutedType) {
  return hasType(qualType(anyOf(substTemplateTypeParmType(),
                                hasDescendant(substTemplateTypeParmType()))));
}

} // namespace

FasterStringFindCheck::FasterStringFindCheck(StringRef Name,
                                             ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      StringLikeClasses(utils::options::parseStringList(
          Options.get("StringLikeClasses", "std::basic_string"))) {
}

void FasterStringFindCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "StringLikeClasses",
                utils::options::serializeStringList(StringLikeClasses));
}

void FasterStringFindCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  const auto SingleChar =
      expr(ignoringParenCasts(stringLiteral(lengthIsOne()).bind("literal")));

  const auto StringFindFunctions =
      anyOf(hasName("find"), hasName("rfind"), hasName("find_first_of"),
            hasName("find_first_not_of"), hasName("find_last_of"),
            hasName("find_last_not_of"));

  llvm::Optional<ast_matchers::internal::Matcher<NamedDecl>> IsStringClass;

  for (const auto &ClassName : StringLikeClasses) {
    const auto HasName = hasName(ClassName);
    IsStringClass = IsStringClass ? anyOf(*IsStringClass, HasName) : HasName;
  }

  if (IsStringClass) {
    Finder->addMatcher(
        cxxMemberCallExpr(
            callee(functionDecl(StringFindFunctions).bind("func")),
            anyOf(argumentCountIs(1), argumentCountIs(2)),
            hasArgument(0, SingleChar),
            on(expr(hasType(recordDecl(*IsStringClass)),
                    unless(hasSubstitutedType())))),
        this);
  }
}

void FasterStringFindCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Literal = Result.Nodes.getNodeAs<StringLiteral>("literal");
  const auto *FindFunc = Result.Nodes.getNodeAs<FunctionDecl>("func");

  auto Replacement = MakeCharacterLiteral(Literal);
  if (!Replacement)
    return;

  diag(Literal->getLocStart(), "%0 called with a string literal consisting of "
                               "a single character; consider using the more "
                               "effective overload accepting a character")
      << FindFunc << FixItHint::CreateReplacement(
                         CharSourceRange::getTokenRange(Literal->getLocStart(),
                                                        Literal->getLocEnd()),
                         *Replacement);
}

} // namespace performance
} // namespace tidy
} // namespace clang
