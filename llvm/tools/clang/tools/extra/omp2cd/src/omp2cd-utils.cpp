#include "include/omp2cd-utils.h"

using namespace llvm;
using namespace clang;
using namespace std;

namespace omp2cd_space {

bool isStmtPrintable(clang::Stmt* s)
{
    int res = false;
    if (s && !(isa<NullStmt>(s)) && isa<ImplicitCastExpr>(s) == false
        && isa<DeclRefExpr>(s) == false && isa<StringLiteral>(s) == false
        && isa<IntegerLiteral>(s) == false && isa<OMPExecutableDirective>(s) == false) {
        res = true;
    }
    return res;
}

string getTypeFromDecl(Decl* decl)
{
    string typeString("");
    ValueDecl* tempValueDecl = dyn_cast<ValueDecl>(decl);
    if (tempValueDecl != nullptr) {
        QualType tempQualType = tempValueDecl->getType();
        typeString = tempQualType.getAsString();
    }
    return typeString;
}

string getNameFromDecl(Decl* decl)
{
    string nameString("");
    NamedDecl* tempNamedDecl = dyn_cast<NamedDecl>(decl);
    if (tempNamedDecl != nullptr) {
        nameString = tempNamedDecl->getNameAsString();
    }
    return nameString;
}
}