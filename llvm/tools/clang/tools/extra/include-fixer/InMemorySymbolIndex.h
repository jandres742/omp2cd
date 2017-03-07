//===-- InMemorySymbolIndex.h -----------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
#ifndef LLVM_CLANG_TOOLS_EXTRA_INCLUDE_FIXER_INMEMORYSYMBOLINDEX_H
#define LLVM_CLANG_TOOLS_EXTRA_INCLUDE_FIXER_INMEMORYSYMBOLINDEX_H

#include "SymbolIndex.h"
#include <map>
#include <string>
#include <vector>

namespace clang {
namespace include_fixer {

/// Xref database with fixed content.
class InMemorySymbolIndex : public SymbolIndex {
public:
  InMemorySymbolIndex(const std::vector<find_all_symbols::SymbolInfo> &Symbols);

  std::vector<clang::find_all_symbols::SymbolInfo>
  search(llvm::StringRef Identifier) override;

private:
  std::map<std::string, std::vector<clang::find_all_symbols::SymbolInfo>>
      LookupTable;
};

} // namespace include_fixer
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_INCLUDE_FIXER_INMEMORYSYMBOLINDEX_H
