#ifndef _OMP2CD_UTILS_H_
#define _OMP2CD_UTILS_H_

#include "clang/AST/Decl.h"
#include "clang/AST/StmtOpenMP.h"

namespace omp2cd_space {

/*Check if a stmt is printable, i.e. we want it in the output file*/
bool isStmtPrintable(clang::Stmt* s);

/*Wrapper to get a decl's type as string from a decl*/
std::string getTypeFromDecl(clang::Decl* decl);

/*Wrapper to get a decl's name as string from a decl*/
std::string getNameFromDecl(clang::Decl* decl);

/*Wrapper for clang's isa to check if s is a stmt's type*/
template <typename T> bool check_isa(clang::Stmt* s)
{
    bool val = false;
    if (s && clang::isa<T>(s))
        val = true;

    return val;
}
}
#endif