//===--- MoveConstandArgumentCheck.cpp - clang-tidy -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MoveConstantArgumentCheck.h"

#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void MoveConstantArgumentCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;
  Finder->addMatcher(callExpr(callee(functionDecl(hasName("::std::move"))),
                              argumentCountIs(1),
                              unless(isInTemplateInstantiation()))
                         .bind("call-move"),
                     this);
}

void MoveConstantArgumentCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *CallMove = Result.Nodes.getNodeAs<CallExpr>("call-move");
  const Expr *Arg = CallMove->getArg(0);
  SourceManager &SM = Result.Context->getSourceManager();

  bool IsConstArg = Arg->getType().isConstQualified();
  bool IsTriviallyCopyable =
      Arg->getType().isTriviallyCopyableType(*Result.Context);

  if (IsConstArg || IsTriviallyCopyable) {
    auto MoveRange = CharSourceRange::getCharRange(CallMove->getSourceRange());
    auto FileMoveRange = Lexer::makeFileCharRange(MoveRange, SM, getLangOpts());
    if (!FileMoveRange.isValid())
      return;
    bool IsVariable = isa<DeclRefExpr>(Arg);
    auto Diag = diag(FileMoveRange.getBegin(),
                     "std::move of the %select{|const }0"
                     "%select{expression|variable}1 "
                     "%select{|of a trivially-copyable type }2"
                     "has no effect; remove std::move()")
                << IsConstArg << IsVariable << IsTriviallyCopyable;

    auto BeforeArgumentsRange = Lexer::makeFileCharRange(
        CharSourceRange::getCharRange(CallMove->getLocStart(),
                                      Arg->getLocStart()),
        SM, getLangOpts());
    auto AfterArgumentsRange = Lexer::makeFileCharRange(
        CharSourceRange::getCharRange(
            CallMove->getLocEnd(), CallMove->getLocEnd().getLocWithOffset(1)),
        SM, getLangOpts());

    if (BeforeArgumentsRange.isValid() && AfterArgumentsRange.isValid()) {
      Diag << FixItHint::CreateRemoval(BeforeArgumentsRange)
           << FixItHint::CreateRemoval(AfterArgumentsRange);
    }
  }
}

} // namespace misc
} // namespace tidy
} // namespace clang
