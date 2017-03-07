#include "include/DFG/LoopInfo.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

LoopInfo::LoopInfo(ASTContext* in_context)
    : initNode(nullptr)
    , condNode(nullptr)
    , incNode(nullptr)
    , initStmt(nullptr)
    , condExpr(nullptr)
    , incrExpr(nullptr)
    , loopVarExpr(nullptr)
    , s(nullptr)
    , loopVarType("")
    , loopVar("")
    , loopCondOpcode("")
    , loopIncrOpcode("")
    , loopInitOpcode("")
    , localLoopVarStr("")
    , transformedLoopVarStr("")
    , initStr("")
    , condStr("")
    , incrStr("")
    , incrVal("")
    , context(in_context)
{
    this->schedulingPolicy.first = STATIC_SCHED;
}

LoopInfo::~LoopInfo() {}

std::string LoopInfo::getForStmtVarRaw()
{
    string loopVar = "";

    if (check_isa<DeclStmt>(this->initStmt)) {
        DeclStmt* initDecl = dyn_cast<DeclStmt>(this->initStmt);

        for (Decl** decls = initDecl->decl_begin(); decls != initDecl->decl_end(); decls++) {
            loopVar = getNameFromDecl(*decls);
        }
    } else if (check_isa<BinaryOperator>(this->initStmt)) {
        BinaryOperator* binOp = dyn_cast<BinaryOperator>(initStmt);
        Expr* lhs = binOp->getLHS();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        lhs->printPretty(rawStringOStream, nullptr, Policy);
        loopVar = rawStringOStream.str();
    }
    return loopVar;
}

std::string LoopInfo::getForStmtChunk(
    omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false)
        parentNode = parentNode->findParentNode();

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string chunkSize = "";

    PrintingPolicy Policy = this->context->getPrintingPolicy();
    Policy.darts_printingInTP = true;
    Policy.darts_TPID = parentRegion->getID();
    Policy.darts_parentNodeID = parentNode->getID();
    Policy.darts_TPInputs = parentNode->ompInputs;
    Policy.darts_TPVars = parentNode->ompVariables;
    Policy.prefixStr = prefixStr;
    Policy.postfixStr = postfixStr;
    Policy.backEnd = backEnd;
    Policy.darts_varTable2 = &(node->myDFG->varTable2);
    vector<int> darts_TPIDTree;
    TPRegion* tempParentRegion = parentRegion;
    while (tempParentRegion) {
        darts_TPIDTree.push_back(tempParentRegion->getID());
        tempParentRegion = tempParentRegion->parent;
    }
    Policy.darts_TPIDTree = darts_TPIDTree;

    string ostreamString;
    raw_string_ostream rawStringOStream(ostreamString);
    Expr* chunkSizeExpr = get<0>(this->schedulingPolicy.second);
    chunkSizeExpr->printPretty(rawStringOStream, nullptr, Policy);
    chunkSize = rawStringOStream.str();

    return chunkSize;
}

std::string LoopInfo::getForStmtVar(
    omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false)
        parentNode = parentNode->findParentNode();

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string loopVar = "";

    if (check_isa<DeclStmt>(this->initStmt)) {
        DeclStmt* initDecl = dyn_cast<DeclStmt>(this->initStmt);

        for (Decl** decls = initDecl->decl_begin(); decls != initDecl->decl_end(); decls++) {
            loopVar = "(";
            loopVar += prefixStr;
            loopVar += getNameFromDecl(*decls);
            loopVar += suffixLang;
            loopVar += to_string(parentNode->getID());
            loopVar += postfixStr;
            loopVar += ")";
        }
    } else if (check_isa<BinaryOperator>(this->initStmt)) {
        BinaryOperator* binOp = dyn_cast<BinaryOperator>(initStmt);
        Expr* lhs = binOp->getLHS();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = true;
        Policy.darts_TPID = parentRegion->getID();
        Policy.darts_parentNodeID = parentNode->getID();
        Policy.darts_TPInputs = parentNode->ompInputs;
        Policy.darts_TPVars = parentNode->ompVariables;
        Policy.prefixStr = prefixStr;
        Policy.postfixStr = postfixStr;
        Policy.backEnd = backEnd;
        Policy.darts_varTable2 = &(node->myDFG->varTable2);
        vector<int> darts_TPIDTree;
        TPRegion* tempParentRegion = parentRegion;
        while (tempParentRegion) {
            darts_TPIDTree.push_back(tempParentRegion->getID());
            tempParentRegion = tempParentRegion->parent;
        }
        Policy.darts_TPIDTree = darts_TPIDTree;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        lhs->printPretty(rawStringOStream, nullptr, Policy);
        loopVar = rawStringOStream.str();
    }
    return loopVar;
}

void LoopInfo::getForStmtStrings(DFGNode* node, std::string& prefixStr, std::string& postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false)
        parentNode = parentNode->findParentNode();

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    /*var*/
    this->transformedLoopVarStr = node->getLoopInfo()->getForStmtVar(node, prefixStr, postfixStr);
    this->loopVar = node->getLoopInfo()->getForStmtVarRaw();
    /*Create the string for the loop's local variable*/
    this->localLoopVarStr = this->loopVar;
    this->localLoopVarStr += suffixLang;
    this->localLoopVarStr += "_counter_temp";
    this->localLoopVarStr += to_string(parentRegion->getID());
    /*cond*/
    this->condStr = node->getLoopInfo()->getForStmtCond(node, prefixStr, postfixStr);
    /*incr*/
    this->incrStr = node->getLoopInfo()->getForStmtIncr(node, prefixStr, postfixStr);

    return;
}

std::string LoopInfo::getForStmtInit(
    omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false)
        parentNode = parentNode->findParentNode();

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string initString = "";
    if (initStmt && isa<DeclStmt>(initStmt)) {
        DeclStmt* initDecl = dyn_cast<DeclStmt>(initStmt);

        for (Decl** decls = initDecl->decl_begin(); decls != initDecl->decl_end(); decls++) {
            if (isa<VarDecl>(*decls)) {
                VarDecl* initVarDecl = dyn_cast<VarDecl>(*decls);
                if (initVarDecl->hasInit()) {
                    Expr* initExpr = initVarDecl->getInit();

                    PrintingPolicy Policy = this->context->getPrintingPolicy();
                    Policy.darts_printingInTP = true;
                    Policy.darts_TPID = parentRegion->getID();
                    Policy.darts_parentNodeID = parentNode->getID();
                    Policy.darts_TPInputs = parentNode->ompInputs;
                    Policy.darts_TPVars = parentNode->ompVariables;
                    Policy.backEnd = backEnd;
                    Policy.prefixStr = prefixStr;
                    if (parentRegion->singleThreaded) {
                        Policy.postfixStr = "";
                    } else {
                        Policy.postfixStr = postfixStr;
                    }
                    Policy.darts_varTable2 = &(node->myDFG->varTable2);
                    vector<int> darts_TPIDTree;
                    TPRegion* tempParentRegion = parentRegion;
                    while (tempParentRegion) {
                        darts_TPIDTree.push_back(tempParentRegion->getID());
                        tempParentRegion = tempParentRegion->parent;
                    }
                    Policy.darts_TPIDTree = darts_TPIDTree;

                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    initExpr->printPretty(rawStringOStream, nullptr, Policy);
                    initString = rawStringOStream.str();
                }
            }
        }
    } else if (check_isa<BinaryOperator>(initStmt)) {
        BinaryOperator* binOp = dyn_cast<BinaryOperator>(initStmt);
        Expr* rhs = binOp->getRHS();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = true;
        Policy.darts_TPID = parentRegion->getID();
        Policy.darts_parentNodeID = parentNode->getID();
        Policy.darts_TPInputs = parentNode->ompInputs;
        Policy.darts_TPVars = parentNode->ompVariables;
        Policy.backEnd = backEnd;
        Policy.prefixStr = prefixStr;
        if (parentRegion->singleThreaded) {
            Policy.postfixStr = "";
        } else {
            Policy.postfixStr = postfixStr;
        }
        Policy.darts_varTable2 = &(node->myDFG->varTable2);
        vector<int> darts_TPIDTree;
        TPRegion* tempParentRegion = parentRegion;
        while (tempParentRegion) {
            darts_TPIDTree.push_back(tempParentRegion->getID());
            tempParentRegion = tempParentRegion->parent;
        }
        Policy.darts_TPIDTree = darts_TPIDTree;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        rhs->printPretty(rawStringOStream, nullptr, Policy);
        initString = rawStringOStream.str();
    }

    return initString;
}

std::string LoopInfo::getForStmtLimit(
    omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false) {
        parentNode = parentNode->findParentNode();
    }

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string limitString;
    if (check_isa<BinaryOperator>(condExpr)) {
        BinaryOperator* condBinOp = dyn_cast<BinaryOperator>(condExpr);
        Expr* rhs = condBinOp->getRHS();

        this->loopCondOpcode = condBinOp->getOpcodeStr();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = true;
        Policy.darts_TPID = parentRegion->getID();
        Policy.darts_parentNodeID = parentNode->getID();
        Policy.darts_TPInputs = parentNode->ompInputs;
        Policy.darts_TPVars = parentNode->ompVariables;
        Policy.backEnd = backEnd;
        Policy.prefixStr = prefixStr;
        if (parentRegion->singleThreaded) {
            Policy.postfixStr = "";
        } else {
            Policy.postfixStr = postfixStr;
        }
        Policy.darts_varTable2 = &(node->myDFG->varTable2);
        vector<int> darts_TPIDTree;
        TPRegion* tempParentRegion = parentRegion;
        while (tempParentRegion) {
            darts_TPIDTree.push_back(tempParentRegion->getID());
            tempParentRegion = tempParentRegion->parent;
        }
        Policy.darts_TPIDTree = darts_TPIDTree;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        rhs->printPretty(rawStringOStream, nullptr, Policy);
        limitString = rawStringOStream.str();
    }
    return limitString;
}

std::string LoopInfo::getForStmtInitRaw()
{
    string initString = "";
    if (initStmt && isa<DeclStmt>(initStmt)) {
        DeclStmt* initDecl = dyn_cast<DeclStmt>(initStmt);

        for (Decl** decls = initDecl->decl_begin(); decls != initDecl->decl_end(); decls++) {
            if (isa<VarDecl>(*decls)) {
                VarDecl* initVarDecl = dyn_cast<VarDecl>(*decls);
                if (initVarDecl->hasInit()) {
                    Expr* initExpr = initVarDecl->getInit();

                    PrintingPolicy Policy = this->context->getPrintingPolicy();
                    Policy.darts_printingInTP = false;

                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    initExpr->printPretty(rawStringOStream, nullptr, Policy);
                    initString = rawStringOStream.str();
                }
            }
        }
    } else if (check_isa<BinaryOperator>(initStmt)) {
        BinaryOperator* binOp = dyn_cast<BinaryOperator>(initStmt);
        Expr* rhs = binOp->getRHS();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        rhs->printPretty(rawStringOStream, nullptr, Policy);
        initString = rawStringOStream.str();
    }

    return initString;
}

std::string LoopInfo::getForStmtLimitRaw()
{
    string limitString;
    if (check_isa<BinaryOperator>(condExpr)) {
        BinaryOperator* condBinOp = dyn_cast<BinaryOperator>(condExpr);
        Expr* rhs = condBinOp->getRHS();

        this->loopCondOpcode = condBinOp->getOpcodeStr();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        rhs->printPretty(rawStringOStream, nullptr, Policy);
        limitString = rawStringOStream.str();
    }
    return limitString;
}

std::string LoopInfo::getForStmtLimitOnInvoke(
    omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false) {
        parentNode = parentNode->findParentNode();
    }

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string limitString;
    if (check_isa<BinaryOperator>(condExpr)) {
        BinaryOperator* condBinOp = dyn_cast<BinaryOperator>(condExpr);
        Expr* rhs = condBinOp->getRHS();

        this->loopCondOpcode = condBinOp->getOpcodeStr();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = true;
        Policy.darts_TPID = parentRegion->getID();
        Policy.darts_parentNodeID = parentNode->getID();
        Policy.darts_TPInputs = parentRegion->mainNode->ompInputs;
        Policy.darts_TPVars = parentRegion->mainNode->ompVariables;
        Policy.backEnd = backEnd;
        Policy.prefixStr = prefixStr;
        if (parentRegion->singleThreaded) {
            Policy.postfixStr = "";
        } else {
            Policy.postfixStr = postfixStr;
        }
        Policy.darts_varTable2 = &(node->myDFG->varTable2);
        vector<int> darts_TPIDTree;
        TPRegion* tempParentRegion = parentRegion;
        while (tempParentRegion) {
            darts_TPIDTree.push_back(tempParentRegion->getID());
            tempParentRegion = tempParentRegion->parent;
        }
        Policy.darts_TPIDTree = darts_TPIDTree;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        rhs->printPretty(rawStringOStream, nullptr, Policy);
        limitString = rawStringOStream.str();
    }
    return limitString;
}

std::string LoopInfo::getForStmtCondRaw()
{
    string condString;
    if (condExpr) {
        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        condExpr->printPretty(rawStringOStream, nullptr, Policy);
        condString = rawStringOStream.str();
    }
    return condString;
}

std::string LoopInfo::getForStmtCond(
    omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false) {
        parentNode = parentNode->findParentNode();
    }

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string condString;

    if (condExpr) {
        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = true;
        Policy.darts_TPID = parentRegion->getID();
        Policy.darts_parentNodeID = parentNode->getID();
        Policy.darts_TPInputs = parentNode->ompInputs;
        Policy.darts_TPVars = parentNode->ompVariables;
        Policy.backEnd = backEnd;
        Policy.prefixStr = prefixStr;
        Policy.postfixStr = postfixStr;
        Policy.darts_varTable2 = &(node->myDFG->varTable2);
        vector<int> darts_TPIDTree;
        TPRegion* tempParentRegion = parentRegion;
        while (tempParentRegion) {
            darts_TPIDTree.push_back(tempParentRegion->getID());
            tempParentRegion = tempParentRegion->parent;
        }
        Policy.darts_TPIDTree = darts_TPIDTree;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        condExpr->printPretty(rawStringOStream, nullptr, Policy);
        condString = rawStringOStream.str();
    }

    return condString;
}

std::string LoopInfo::getForStmtCondWhile(
    omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr)
{
    DFGNode* parentNode = node->findParentNode();

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string condString;

    if (condExpr && parentNode) {
        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = true;
        Policy.darts_TPID = parentRegion->getID();
        Policy.darts_parentNodeID = parentNode->getID();
        Policy.darts_TPInputs = parentNode->ompInputs;
        Policy.darts_TPVars = parentNode->ompVariables;
        Policy.backEnd = backEnd;
        Policy.prefixStr = prefixStr;
        Policy.postfixStr = postfixStr;
        Policy.darts_varTable2 = &(node->myDFG->varTable2);
        vector<int> darts_TPIDTree;
        TPRegion* tempParentRegion = parentRegion;
        while (tempParentRegion) {
            darts_TPIDTree.push_back(tempParentRegion->getID());
            tempParentRegion = tempParentRegion->parent;
        }
        Policy.darts_TPIDTree = darts_TPIDTree;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        condExpr->printPretty(rawStringOStream, nullptr, Policy);
        condString = rawStringOStream.str();
    }

    return condString;
}

std::string LoopInfo::getForStmtIncrRaw()
{
    string incrString;

    if (incrExpr) {
        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        incrExpr->printPretty(rawStringOStream, nullptr, Policy);
        incrString = rawStringOStream.str();
    }

    return incrString;
}

std::string LoopInfo::getForStmtIncr(
    omp2cd_space::DFGNode* node, std::string& prefixStr, std::string& postfixStr)
{
    DFGNode* parentNode = node;
    if (check_isa<OMPExecutableDirective>(parentNode->getStmt()) == false) {
        parentNode = parentNode->findParentNode();
    }

    TPRegion* parentRegion = node->getRegion()->findOMPOrFunctionRegionParent();

    string incrString;

    if (incrExpr) {
        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = true;
        Policy.darts_TPID = parentRegion->getID();
        Policy.darts_parentNodeID = parentNode->getID();
        Policy.darts_TPInputs = parentNode->ompInputs;
        Policy.darts_TPVars = parentNode->ompVariables;
        Policy.backEnd = backEnd;
        Policy.prefixStr = prefixStr;
        Policy.postfixStr = postfixStr;
        Policy.darts_varTable2 = &(node->myDFG->varTable2);
        vector<int> darts_TPIDTree;
        TPRegion* tempParentRegion = parentRegion;
        while (tempParentRegion) {
            darts_TPIDTree.push_back(tempParentRegion->getID());
            tempParentRegion = tempParentRegion->parent;
        }
        Policy.darts_TPIDTree = darts_TPIDTree;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        incrExpr->printPretty(rawStringOStream, nullptr, Policy);
        incrString = rawStringOStream.str();
    }

    return incrString;
}

void LoopInfo::getForStmtCondOpCode()
{
    if (condExpr && isa<BinaryOperator>(condExpr)) {
        BinaryOperator* condBinOp = dyn_cast<BinaryOperator>(condExpr);
        this->loopCondOpcode = condBinOp->getOpcodeStr();
    }
}

void LoopInfo::initForStmtInfo(
    ForStmt* s, omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr)
{
    DFGNode* parentNode = node;
    while (check_isa<OMPForDirective>(parentNode->getStmt()) == false
        && check_isa<OMPParallelForDirective>(parentNode->getStmt()) == false
        && parentNode->myFunctionInfo == nullptr) {
        parentNode = parentNode->parent;
    }

    // init
    this->initStmt = s->getInit();

    if (check_isa<DeclStmt>(this->initStmt)) {
        DeclStmt* initDecl = dyn_cast<DeclStmt>(this->initStmt);

        for (Decl** decls = initDecl->decl_begin(); decls != initDecl->decl_end(); decls++) {
            this->loopVarType = getTypeFromDecl(*decls);
        }
    } else if (check_isa<BinaryOperator>(this->initStmt)) {
        BinaryOperator* binOp = dyn_cast<BinaryOperator>(initStmt);
        Expr* lhs = binOp->getLHS();

        QualType tempQualType = lhs->getType();
        this->loopVarType = tempQualType.getAsString();
    }
    // cond
    this->condExpr = s->getCond();
    this->getForStmtCondOpCode();

    // incr
    this->incrExpr = s->getInc();
    if (check_isa<UnaryOperator>(this->incrExpr)) {
        this->incrVal = "1";
    } else if (check_isa<CompoundAssignOperator>(this->incrExpr)) {
        BinaryOperator* binOp = dyn_cast<BinaryOperator>(this->incrExpr);
        Expr* rhs = binOp->getRHS();

        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        rhs->printPretty(rawStringOStream, nullptr, Policy);
        this->incrVal = rawStringOStream.str();
    } else if (check_isa<BinaryOperator>(this->incrExpr)) {
        /*TODO: incr expression of the form i = i + NUM. We need to be able
         * to extrct the NUM.
         **/
    }
}

void LoopInfo::initWhileStmtInfo(
    WhileStmt* s, omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr)
{
    // cond
    this->condExpr = s->getCond();
    this->getForStmtCondOpCode();
}

void LoopInfo::initDoStmtInfo(
    DoStmt* s, omp2cd_space::DFGNode* node, std::string prefixStr, std::string postfixStr)
{
    // cond
    this->condExpr = s->getCond();
    this->getForStmtCondOpCode();
}

void LoopInfo::printRequestIterationsFunctionDef(
    omp2cd_space::DFGNode* loopNode, std::ostringstream& outputStream)
{
    string loopVarType = this->loopVarType;
    outputStream << "bool requestNewRangeIterations" << loopNode->getID() << "(" << loopVarType
                 << "* endRange, uint32_t codeletID";

    bool atLeastOneLastPrivate = false;
    for (auto var : loopNode->ompInputs) {
        if (get<1>(var.second) == OMP_LASTPRIVATE) {
            atLeastOneLastPrivate = true;
            break;
        }
    }

    if (atLeastOneLastPrivate)
        outputStream << ", bool *containsLastIteration";

    outputStream << ");\n";
}

void LoopInfo::printRequestIterationsFunctionImpl(
    omp2cd_space::DFGNode* loopNode, std::ostringstream& outputStream, int parentRegionID)
{
    bool atLeastOneLastPrivate = false;
    for (auto var : loopNode->ompInputs) {
        if (get<1>(var.second) == OMP_LASTPRIVATE) {
            atLeastOneLastPrivate = true;
            break;
        }
    }

    string loopInit = "";
    string loopLimit = "";

    if (DARTS_BACKEND) {
        loopInit = this->getForStmtInit(loopNode, "this->", "[this->getID()]");
        loopLimit = this->getForStmtLimit(loopNode, "this->", "[this->getID()]");
    } else if (SWARM_BACKEND) {
        loopInit = this->getForStmtInit(loopNode, "", "[codeletID]");
        loopLimit = this->getForStmtLimit(loopNode, "", "[codeletID]");
    }

    string chunkSize = "";
    if (get<0>(this->schedulingPolicy.second) == nullptr) {
        chunkSize = to_string(get<1>(this->schedulingPolicy.second));
    } else {
        if (DARTS_BACKEND)
            chunkSize = this->getForStmtChunk(loopNode, "this->inputsTPParent->", "[codeletID]");
        else if (SWARM_BACKEND)
            chunkSize = this->getForStmtChunk(loopNode, "", "[codeletID]");
    }

    string loopVar = "";
    if (DARTS_BACKEND)
        loopVar = this->getForStmtVar(loopNode, "this->inputsTPParent->", "[codeletID]");
    else if (SWARM_BACKEND)
        loopVar = this->getForStmtVar(loopNode, "", "[codeletID]");

    if (DARTS_BACKEND) {
        outputStream << "bool TP" << parentRegionID << "::requestNewRangeIterations"
                     << loopNode->getID() << "(" << this->loopVarType
                     << "* endRange, uint32_t codeletID";

        if (atLeastOneLastPrivate)
            outputStream << ", bool *containsLastIteration";
    } else if (SWARM_BACKEND) {
        outputStream << "bool requestNewRangeIterations" << loopNode->getID() << "("
                     << this->loopVarType << " *startRange, " << this->loopVarType
                     << " *endRange, uint32_t codeletID";

        if (atLeastOneLastPrivate)
            outputStream << ", bool *containsLastIteration";
    }

    outputStream << ")\n{\n";

    outputStream << "/*Scheduling Policy = ";
    if (get<0>(this->schedulingPolicy) == 0)
        outputStream << "Static ";
    else if (get<0>(this->schedulingPolicy) == 1)
        outputStream << "Dynamic ";
    else if (get<0>(this->schedulingPolicy) == 2)
        outputStream << "Guided ";
    else if (get<0>(this->schedulingPolicy) == 3)
        outputStream << "Auto ";
    else
        outputStream << "Runtime ";

    outputStream << "*/\n";
    outputStream << "/*Chunk = " << chunkSize << "*/\n";

    outputStream << "bool isThereNewIteration = false;\n";

    /*If you change something in this code, change it also in printRegionFireCode where the init and
     * last
     * iteration for each region of a balanced OMPFor are calculated. That code is pretty much the
     * same
     * as this one for a static scheduling.*/
    if (get<0>(this->schedulingPolicy) == STATIC_SCHED
        || get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
        if (get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
            outputStream << "if(ompLoopSched" << loopNode->getID() << " == 1)\n";
        }
        outputStream << "{\n";
        outputStream << "/*Static Scheduling*/\n";

        outputStream << this->loopVarType << " tempStartRange = rangePerCodelet"
                     << loopNode->getID() << " * codeletID;\n";
        outputStream << this->loopVarType << " tempEndRange   = rangePerCodelet"
                     << loopNode->getID() << " * (codeletID + 1);\n";

        outputStream << "if (remainderRange" << loopNode->getID() << " != 0)\n";
        outputStream << "{\n";
        outputStream << "if (codeletID < (uint32_t)remainderRange" << loopNode->getID() << ")\n";
        outputStream << "{\n";
        outputStream << "tempStartRange += codeletID;\n";
        outputStream << "tempEndRange += (codeletID + 1);\n";
        outputStream << "}\n";
        outputStream << "else\n";
        outputStream << "{\n";
        outputStream << "tempStartRange += remainderRange" << loopNode->getID() << ";\n";
        outputStream << "tempEndRange += remainderRange" << loopNode->getID() << ";\n";
        outputStream << "}\n";
        outputStream << "}\n";

        outputStream << "tempStartRange = tempStartRange*" << this->incrVal << " + minIteration"
                     << loopNode->getID() << ";\n";
        outputStream << "tempEndRange = tempEndRange*" << this->incrVal << " + minIteration"
                     << loopNode->getID() << ";\n";

        outputStream << "if(tempStartRange != tempEndRange)\n";
        outputStream << "{\n";
        outputStream << "isThereNewIteration = true;\n";
        outputStream << "}\n";

        outputStream << "if(initIteration" << loopNode->getID() << " < lastIteration"
                     << loopNode->getID() << ")\n";
        outputStream << "{\n";

        if (DARTS_BACKEND)
            outputStream << loopVar << " = min(tempStartRange, tempEndRange);\n";
        else if (SWARM_BACKEND)
            outputStream << "*startRange = min(tempStartRange, tempEndRange);\n";

        outputStream << "*endRange   = max(tempStartRange, tempEndRange);\n";
        outputStream << "}\n";
        outputStream << "else\n";
        outputStream << "{\n";

        if (DARTS_BACKEND)
            outputStream << loopVar << " = max(tempStartRange, tempEndRange);\n";
        else if (SWARM_BACKEND)
            outputStream << "*startRange = max(tempStartRange, tempEndRange);\n";

        outputStream << "*endRange   = min(tempStartRange, tempEndRange);\n";
        outputStream << "}\n";

        string loopCondOpcode = this->loopCondOpcode;
        size_t posEqual = loopCondOpcode.find("=");
        if (posEqual != string::npos) {
            size_t posLessThanEqual = loopCondOpcode.find("<=");
            if (posLessThanEqual != string::npos) {
                if (DARTS_BACKEND)
                    outputStream << "if(codeletID == this->numThreads - 1)\n";
                else if (SWARM_BACKEND)
                    outputStream << "if(codeletID == numThreads" << loopNode->getID() << " - 1)\n";

                outputStream << "{\n";
                outputStream << "*endRange = *endRange + 1;\n";
                outputStream << "}\n";
            } else {
                outputStream << "if(codeletID == 0)\n";
                outputStream << "{\n";
                outputStream << "*endRange = *endRange - 1;\n";
                outputStream << "}\n";
            }
        }

        if (DARTS_BACKEND)
            outputStream << "if(codeletID == this->numThreads - 1)\n";
        else if (SWARM_BACKEND)
            outputStream << "if(codeletID == numThreads" << loopNode->getID() << " - 1)\n";

        outputStream << "{\n";
        outputStream << "*endRange = lastIteration" << loopNode->getID() << ";\n";
        outputStream << "}\n";
        outputStream << "}\n";
    }

    if (get<0>(this->schedulingPolicy) == DYNAMIC_SCHED
        || get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {

        string loopCondOpcode = this->loopCondOpcode;

        if (get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
            chunkSize = "ompLoopChunk";
            chunkSize += to_string(loopNode->getID());
            outputStream << "else if(ompLoopSched" << loopNode->getID() << " == 2)\n";
        }
        outputStream << "{\n";
        outputStream << "/*Dynamic Scheduling*/\n";

        outputStream << "if(initIteration" << loopNode->getID() << " < lastIteration"
                     << loopNode->getID() << ")\n";
        outputStream << "{\n";

        if (DARTS_BACKEND)
            outputStream << loopVar << " = __sync_fetch_and_add(&(nextIteration"
                         << loopNode->getID() << "), " << chunkSize << "*" << this->incrVal
                         << ");\n";
        else if (SWARM_BACKEND)
            outputStream << "*startRange = __sync_fetch_and_add(&(nextIteration"
                         << loopNode->getID() << "), " << chunkSize << "*" << this->incrVal
                         << ");\n";

        if (DARTS_BACKEND)
            outputStream << "*endRange = " << loopVar << " + " << chunkSize << "*" << this->incrVal
                         << ";\n";
        else if (SWARM_BACKEND)
            outputStream << "*endRange = *startRange + " << chunkSize << "*" << this->incrVal
                         << ";\n";

        outputStream << "}\n";
        outputStream << "else\n";
        outputStream << "{\n";

        if (DARTS_BACKEND)
            outputStream << loopVar << " = __sync_fetch_and_sub(&(nextIteration"
                         << loopNode->getID() << "), " << chunkSize << "*" << this->incrVal
                         << ");\n";
        else if (SWARM_BACKEND)
            outputStream << "*startRange = __sync_fetch_and_sub(&(nextIteration"
                         << loopNode->getID() << "), " << chunkSize << "*" << this->incrVal
                         << ");\n";

        if (DARTS_BACKEND)
            outputStream << "*endRange = " << loopVar << " - " << chunkSize << "*" << this->incrVal
                         << ";\n";
        else if (SWARM_BACKEND)
            outputStream << "*endRange = *startRange - " << chunkSize << "*" << this->incrVal
                         << ";\n";

        outputStream << "}\n";

        if (DARTS_BACKEND)
            outputStream << "if(" << loopVar << " " << loopCondOpcode << " lastIteration"
                         << loopNode->getID() << ")\n";
        else if (SWARM_BACKEND)
            outputStream << "if(*startRange " << loopCondOpcode << " lastIteration"
                         << loopNode->getID() << ")\n";

        outputStream << "{\n";
        outputStream << "isThereNewIteration = true;\n";
        outputStream << "}\n";
        outputStream << "}\n";
    }

    if (atLeastOneLastPrivate) {
        if (DARTS_BACKEND)
            outputStream << "if(" << loopVar << " <= lastIteration" << loopNode->getID()
                         << " && lastIteration" << loopNode->getID() << " <= *endRange){\n"
                         << "*containsLastIteration = true;\n"
                         << "}\n";
        else if (SWARM_BACKEND)
            outputStream << "if(*startRange <= lastIteration" << loopNode->getID()
                         << " && lastIteration" << loopNode->getID() << " <= *endRange){\n"
                         << "*containsLastIteration = true;\n"
                         << "}\n";
    }

    outputStream << "return isThereNewIteration;\n";

    outputStream << "}\n";
}

void LoopInfo::implementDARTSLoops(omp2cd_space::TPRegion* region,
    omp2cd_space::TPRegion* parentRegion, std::ostringstream& fireStmtStream)
{
    string loopInit
        = this->getForStmtInit(parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, "[this->getID()]");
    string loopLimit
        = this->getForStmtLimit(parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, "[this->getID()]");

    fireStmtStream << this->loopVarType << " range = abs(" << loopLimit << " - " << loopInit
                   << ");\n"
                   << "unsigned int workers = (ompNumThreads / NUMTPS - 1) + 1;\n"
                   << "if(range < dartsTPLoopTH0)\n"
                   << "{\n"
                   << "_checkInCodelets" << region->getID() << "_core  *tempCodeletCore ="
                   << "new _checkInCodelets" << region->getID()
                   << "_core (1, 1, myTP,&(myTP->barrierCodelets" << region->getID() << "[0])"
                   << ", " << loopInit << ", " << loopLimit
                   << region->mainNode->printOMPInputNamesInFunctionCall(
                          parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "[this->getID()]")
                   << ");\n"
                   << "myTP->masterCodeletTP" << region->getID() << "_core = tempCodeletCore;\n"
                   << "myTP->masterCodeletTP" << region->getID() << "_core[0].decDep();\n"
                   << "}\n"
                   << "else if(range < dartsTPLoopTH1)\n"
                   << "{\n"
                   << "uint64_t outerTile = (range/workers) ? (range/workers) : range;\n";

    fireStmtStream << "codeletFor < TP" << region->getID() << "_loop > *tempCodeletFor ="
                   << "new codeletFor < TP" << region->getID() << "_loop >"
                   << "(1, 1, myTP, LONGWAIT, &(myTP->barrierCodelets" << region->getID()
                   << "[0]), 1, outerTile"
                   << ", " << loopInit << ", " << loopLimit
                   << region->mainNode->printOMPInputNamesInFunctionCall(
                          parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "[this->getID()]")
                   << ");\n"
                   << "tempCodeletFor->setIterations( "
                   << " (range/outerTile + ( (range%outerTile) ? 1 : 0) ) );\n"
                   << "myTP->masterCodeletTP" << region->getID() << "_loop = tempCodeletFor;\n"
                   << "myTP->masterCodeletTP" << region->getID() << "_loop[0].decDep();\n"
                   << "}\n"
                   << "else\n"
                   << "{\n"
                   << "uint64_t outerTile = (range/(NUMTPS * 2));\n"
                   << "if(outerTile == 0){\n"
                   << "outerTile = (range/NUMTPS);\n"
                   << "if(outerTile == 0)\n"
                   << "outerTile = range;\n"
                   << "}\n";

    fireStmtStream << "paraFor < TP" << region->getID() << "_outer > *tempParaFor ="
                   << "new paraFor < TP" << region->getID() << "_outer >"
                   << "(1, 1, myTP, LONGWAIT, &(myTP->barrierCodelets" << region->getID()
                   << "[0]), 1, outerTile, workers"
                   << ", " << loopInit << ", " << loopLimit
                   << region->mainNode->printOMPInputNamesInFunctionCall(
                          parentRegion->mainNode, DARTS_PREFIXSTR_DEFAULT, true, "[this->getID()]")
                   << ");\n"
                   << "tempParaFor->setIterations("
                   << "(range/outerTile + ( (range%outerTile) ? 1 : 0) ) );\n"
                   << "myTP->masterCodeletTP" << region->getID() << "_outer = tempParaFor;\n"
                   << "myTP->masterCodeletTP" << region->getID() << "_outer[0].decDep();\n"
                   << "}\n";
}

void LoopInfo::printRequestIterationsFunctionDefTPLoops(
    omp2cd_space::DFGNode* loopNode, std::ostringstream& outputStream)
{
    string loopVarType = this->loopVarType;
    outputStream << "bool requestNewRangeIterations" << loopNode->getID() << "(" << loopVarType
                 << " &startRange, " << loopVarType << " &endRange, const uint32_t codeletID";

    bool atLeastOneLastPrivate = false;
    for (auto var : loopNode->ompInputs) {
        if (get<1>(var.second) == OMP_LASTPRIVATE) {
            atLeastOneLastPrivate = true;
            break;
        }
    }

    if (atLeastOneLastPrivate)
        outputStream << ", bool *containsLastIteration";

    outputStream << ", bool scheduling = 0);";
}

void LoopInfo::printRequestIterationsFunctionImplTPLoops(
    omp2cd_space::DFGNode* loopNode, std::ostringstream& outputStream, int parentRegionID)
{
    bool atLeastOneLastPrivate = false;
    for (auto var : loopNode->ompInputs) {
        if (get<1>(var.second) == OMP_LASTPRIVATE) {
            atLeastOneLastPrivate = true;
            break;
        }
    }

    string loopInit = this->getForStmtInit(loopNode, "this->", "[this->getID()]");
    string loopLimit = this->getForStmtLimit(loopNode, "this->", "[this->getID()]");

    string chunkSize = "";
    if (get<0>(this->schedulingPolicy.second) == nullptr)
        chunkSize = to_string(get<1>(this->schedulingPolicy.second));
    else
        chunkSize = this->getForStmtChunk(loopNode, "this->inputsTPParent->", "[codeletID]");

    string loopVar = this->getForStmtVar(loopNode, "this->inputsTPParent->", "[codeletID]");

    outputStream << "bool TP" << parentRegionID << "::requestNewRangeIterations"
                 << loopNode->getID() << "(" << loopVarType << " &startRange, " << loopVarType
                 << " &endRange, const uint32_t codeletID";

    if (atLeastOneLastPrivate)
        outputStream << ", bool *containsLastIteration";

    outputStream << ", bool scheduling)\n{\n";

    outputStream << "bool isThereNewIteration = false;\n";

    outputStream << "/*Scheduling Policy = ";
    if (get<0>(this->schedulingPolicy) == 0) {
        outputStream << "Static ";
    } else if (get<0>(this->schedulingPolicy) == 1) {
        outputStream << "Dynamic ";
    } else if (get<0>(this->schedulingPolicy) == 2) {
        outputStream << "Guided ";
    } else if (get<0>(this->schedulingPolicy) == 3) {
        outputStream << "Auto ";
    } else {
        outputStream << "Runtime ";
    }
    outputStream << "*/\n";
    outputStream << "/*Chunk = " << chunkSize << "*/\n";

    /*If you change something in this code, change it also in printRegionFireCode where the init and
     * last
     * iteration for each region of a balanced OMPFor are calculated. That code is pretty much the
     * same
     * as this one for a static scheduling.*/
    if (get<0>(this->schedulingPolicy) == STATIC_SCHED
        || get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
        if (get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
            outputStream << "if(this->ompLoopSched == 1)\n";
        }
        outputStream << "{\n";
        outputStream << "/*Static Scheduling*/\n";

        outputStream << this->loopVarType << " tempStartRange = rangePerCodelet * codeletID;\n";
        outputStream << this->loopVarType
                     << " tempEndRange   = rangePerCodelet * (codeletID + 1);\n";

        outputStream << "if (remainderRange != 0)\n";
        outputStream << "{\n";
        outputStream << "if (codeletID < (uint32_t)remainderRange)\n";
        outputStream << "{\n";
        outputStream << "tempStartRange += codeletID;\n";
        outputStream << "tempEndRange += (codeletID + 1);\n";
        outputStream << "}\n";
        outputStream << "else\n";
        outputStream << "{\n";
        outputStream << "tempStartRange += remainderRange;\n";
        outputStream << "tempEndRange += remainderRange;\n";
        outputStream << "}\n";
        outputStream << "}\n";

        outputStream << "tempStartRange = tempStartRange*" << this->incrVal << " + minIteration;\n";
        outputStream << "tempEndRange = tempEndRange*" << this->incrVal << " + minIteration;\n";

        outputStream << "if(tempStartRange != tempEndRange)\n";
        outputStream << "{\n";
        outputStream << "isThereNewIteration = true;\n";
        outputStream << "}\n";

        outputStream << "if(this->initIteration < this->lastIteration)\n";
        outputStream << "{\n";
        outputStream << "startRange = min(tempStartRange, tempEndRange);\n";
        outputStream << "endRange   = max(tempStartRange, tempEndRange);\n";
        outputStream << "}\n";
        outputStream << "else\n";
        outputStream << "{\n";
        outputStream << "startRange = max(tempStartRange, tempEndRange);\n";
        outputStream << "endRange   = min(tempStartRange, tempEndRange);\n";
        outputStream << "}\n";

        string loopCondOpcode = this->loopCondOpcode;
        size_t posEqual = loopCondOpcode.find("=");
        if (posEqual != string::npos) {
            size_t posLessThanEqual = loopCondOpcode.find("<=");
            if (posLessThanEqual != string::npos) {
                outputStream << "if(codeletID == this->totalCodelets - 1)\n";
                outputStream << "{\n";
                outputStream << "endRange = endRange + 1;\n";
                outputStream << "}\n";
            } else {
                outputStream << "if(codeletID == 0)\n";
                outputStream << "{\n";
                outputStream << "endRange = endRange - 1;\n";
                outputStream << "}\n";
            }
        }

        outputStream << "if(codeletID == this->totalCodelets - 1)\n";
        outputStream << "{\n";
        outputStream << "endRange = this->lastIteration;\n";
        outputStream << "}\n";
        outputStream << "}\n";
    }

    /*TODO: Test if this code works. In particular, we need to test the nextIteration.
     * That variable should be the same for all the TPLoops handled by the same
     * master codelet.*/
    if (get<0>(this->schedulingPolicy) == DYNAMIC_SCHED
        || get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
        // string chunkSize  = to_string(get<1>(this->schedulingPolicy));
        string loopCondOpcode = this->loopCondOpcode;

        if (get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
            chunkSize = "this->ompLoopChunk";
            outputStream << "else if(this->ompLoopSched == 2)\n";
        }
        outputStream << "{\n";
        outputStream << "/*Dynamic Scheduling*/\n";

        outputStream << "if(this->initIteration < this->lastIteration)\n";
        outputStream << "{\n";
        outputStream << loopVar << " = __sync_fetch_and_add(&(this->nextIteration), " << chunkSize
                     << "*" << this->incrVal << ");\n";
        outputStream << "endRange = " << loopVar << " + " << chunkSize << "*" << this->incrVal
                     << ";\n";
        outputStream << "}\n";
        outputStream << "else\n";
        outputStream << "{\n";
        outputStream << loopVar << " = __sync_fetch_and_sub(&(this->nextIteration), " << chunkSize
                     << "*" << this->incrVal << ");\n";
        outputStream << "endRange = " << loopVar << " - " << chunkSize << "*" << this->incrVal
                     << ";\n";
        outputStream << "}\n";
        outputStream << "if(" << loopVar << " " << loopCondOpcode << " this->lastIteration)\n";
        outputStream << "{\n";
        outputStream << "isThereNewIteration = true;\n";
        outputStream << "}\n";
        outputStream << "}\n";
    }

    if (atLeastOneLastPrivate)
        outputStream << "if(" << loopVar << " <= this->lastIteration" << loopNode->getID()
                     << " && this->lastIteration" << loopNode->getID() << " <= *endRange){\n"
                     << "*containsLastIteration = true;\n"
                     << "}\n";

    outputStream << "return isThereNewIteration;\n";

    outputStream << "}\n";
}

std::string LoopInfo::getReductionVarInitVal(std::string opCode, std::string& varType)
{
    /*Reduction variable local copy according to the OpenMP 3.1 standard, pp 103 */
    string initValue = "";
    if (strcmp(opCode.c_str(), "+") == 0) {
        initValue = "0";
    } else if (strcmp(opCode.c_str(), "-") == 0) {
        initValue = "0";
    } else if (strcmp(opCode.c_str(), "*") == 0) {
        initValue = "1";
    } else if (strcmp(opCode.c_str(), "&") == 0) {
        initValue = "~0";
    } else if (strcmp(opCode.c_str(), "|") == 0) {
        initValue = "0";
    } else if (strcmp(opCode.c_str(), "^") == 0) {
        initValue = "0";
    } else if (strcmp(opCode.c_str(), "&&") == 0) {
        initValue = "1";
    } else if (strcmp(opCode.c_str(), "||") == 0) {
        initValue = "0";
    } else if (strcmp(opCode.c_str(), "max") == 0) {
        if (DARTS_BACKEND) {
            initValue = "(std::numeric_limits<" + varType + ">::infinity() * (-1))";
        } else if (SWARM_BACKEND) {
            if (varType.find("char") != string::npos)
                initValue = "CHAR_MIN";
            else if (varType.find("int") != string::npos)
                initValue = "INT_MIN";
            else if (varType.find("long") != string::npos)
                initValue = "LONG_MIN";
            else if (varType.find("float") != string::npos)
                initValue = "-FLOAT_MAX";
            // else if (varType.find("double") != string::npos)
            else
                initValue = "-DBL_MAX";
        }
    } else if (strcmp(opCode.c_str(), "min") == 0) {
        if (DARTS_BACKEND) {
            initValue = "(std::numeric_limits<" + varType + ">::infinity())";
        } else if (SWARM_BACKEND) {
            if (varType.find("char") != string::npos)
                initValue = "CHAR_MAX";
            else if (varType.find("int") != string::npos)
                initValue = "INT_MAX";
            else if (varType.find("long") != string::npos)
                initValue = "LONG_MAX";
            else if (varType.find("float") != string::npos)
                initValue = "FLOAT_MAX";
            // else if(varType.find("double") != string::npos)
            else
                initValue = "DBL_MAX";
        }
    }
    return initValue;
}

void LoopInfo::printCalcOfParamsForBalancedOMPFor(
    TPRegion* region, std::ostringstream& outputStream)
{
    TPRegion* parentRegion = region->parent;

    string loopInit = "";
    string loopLimit = "";

    /*Set the init and last iteration for an OMPFor */
    if (region->isOMPFor()) {
        DFGNode* srcNode = parentRegion->mainNode;
        if (!(check_isa<OMPExecutableDirective>(srcNode->getStmt()))) {
            srcNode = srcNode->findParentNode();
        }
        loopInit = this->getForStmtInit(srcNode, DARTS_PREFIXSTR_DEFAULT, "[this->getID()]");
        loopLimit = this->getForStmtLimit(srcNode, DARTS_PREFIXSTR_DEFAULT, "[this->getID()]");
    }

/*TODO: This is not working properly and in some cases produces segfault.*/
#if 0
	outputStream <<
		"/*Check for the loop's range and modify the number and size of TPs to launch*/\n"
		<< "if (!__sync_val_compare_and_swap (&(myTP->numTPsSet" << region->
		getID() << "), 0, 1))\n" << "{\n" << this->
		loopVarType << " range = " << loopLimit << " - " << loopInit << ";\n"
#if 1
		<< "if(range < RANGELIMIT" << region->getID() << "_0)\n" << "{\n"
#endif
		<< "myTP->codeletsPerTP" << region->getID() << " = 1;\n"
		<< "myTP->TPsToUse" << region->getID() << " = 1;\n"
#if 1
		<< "}\n"
		<< "else if(range < RANGELIMIT" << region->getID() << "_1)\n"
		<< "{\n"
		<< "myTP->codeletsPerTP" << region->getID() << " /= 2;\n"
		<< "myTP->TPsToUse" << region->getID() << " /= 2;\n"
		<< "if(myTP->codeletsPerTP" << region->getID() << " <= 0)\n"
		<< "myTP->codeletsPerTP" << region->getID() << " = 1;\n"
		<< "if(myTP->TPsToUse" << region->getID() << " <= 0)\n"
		<< "myTP->TPsToUse" << region->getID() << " = 1;\n" << "}\n";
#endif

	if (region->hasBarrier()) {
		outputStream << "myTP->barrierCodelets" << region->
			getID() << "[0].setDep(myTP->TPsToUse" << region->getID() << ");\n";
	}

	outputStream << "myTP->totalCodelets" << region->getID() << " = myTP->TPsToUse"
		<< region->getID() << " * myTP->codeletsPerTP" << region->getID() << ";\n"
		<< "myTP->numTPsReady" << region->getID() << " = 1;\n"
		<< "}\n"
		<< "/*Reset if the number and size of TPs are not ready yet*/\n"
		<< "if(myTP->numTPsReady" << region->getID() << " == 0)\n"
		<< "{\n"
		<< "this->resetCodelet ();\n"
		<< "this->decDep ();\n"
		<< "return;\n"
		<< "}\n";
#endif

    /*If you change something in region code, change it also in
     * LoopInfo::printRequestIterationsFunctionImpl
     * where the init and last iteration for each region of a balanced OMPFor are calculated.
     * That code is pretty much the same as region one for a static scheduling.*/

    outputStream << this->loopVarType << " range = "
                 << "abs (" << loopLimit << " - " << loopInit << ") / " << this->incrVal << ";\n"
                 << this->loopVarType << " rangePerCodelet = range / myTP->TPsToUse"
                 << region->getID() << ";\n"
                 << this->loopVarType << " minIteration = min<" << this->loopVarType << " >("
                 << loopLimit << ", " << loopInit << ");\n"
                 << this->loopVarType << " remainderRange = range % myTP->TPsToUse"
                 << region->getID() << ";\n"
                 << this->loopVarType << " initIteration = rangePerCodelet * idx;\n"
                 << this->loopVarType << " lastIteration = rangePerCodelet * (idx + 1);\n"
                 << "if (remainderRange != 0)\n"
                 << "{\n"
                 << "if (idx < (uint32_t)remainderRange)\n"
                 << "{\n"
                 << "initIteration += idx;\n"
                 << "lastIteration += (idx + 1);\n"
                 << "}\n"
                 << "else\n"
                 << "{\n"
                 << "initIteration += remainderRange;\n"
                 << "lastIteration += remainderRange;\n"
                 << "}\n"
                 << "}\n"
                 << "initIteration = initIteration*" << this->incrVal << " + minIteration;\n"
                 << "lastIteration = lastIteration*" << this->incrVal << " + minIteration;\n"
                 << "if(" << loopInit << " < " << loopLimit << ")\n"
                 << "{\n"
                 << "initIteration = min(initIteration, lastIteration);\n"
                 << "lastIteration = max(initIteration, lastIteration);\n"
                 << "}\n"
                 << "else\n"
                 << "{\n"
                 << "initIteration = max(initIteration, lastIteration);\n"
                 << "lastIteration = min(initIteration, lastIteration);\n"
                 << "}\n";

    string loopCondOpcode = this->loopCondOpcode;
    size_t posEqual = loopCondOpcode.find("=");
    if (posEqual != string::npos) {
        size_t posLessThanEqual = loopCondOpcode.find("<=");
        if (posLessThanEqual != string::npos) {
            outputStream << "if(idx == myTP->TPsToUse" << region->getID() << " - 1)\n"
                         << "{\n"
                         << "lastIteration = lastIteration + 1;\n"
                         << "}\n";
        } else {
            outputStream << "if(idx == 0)\n"
                         << "{\n"
                         << "lastIteration = lastIteration - 1;\n"
                         << "}\n";
        }
    }

    outputStream << "if(idx == myTP->TPsToUse" << region->getID() << " - 1)\n"
                 << "{\n"
                 << "lastIteration = " << loopLimit << ";\n"
                 << "}\n";
}

void LoopInfo::findVarsToChange(std::vector<std::string>& varNamesToDeclare,
    std::vector<std::string>& varNamesToChange, std::vector<std::string>& varTypesToChange,
    std::vector<std::string>& dereferenceSymbol, omp2cd_space::TPRegion* parentRegion,
    DFGNode* parentNode, std::ostringstream& ptrsStream, std::string& forStmtString,
    std::string& forBodyString)
{

    std::vector<std::string> varArraysToChange;
    bool isSingleThreaded = parentRegion->singleThreaded;

#if FULL_DEBUG_COMMENTS == 1
    ptrsStream << "/*parentRegion: " << parentRegion->getName() << " " << parentRegion->getID()
               << "*/\n";
#endif

    for (auto var : parentRegion->getOMPInputs()) {

        bool isReductionVar = false;
        if (parentRegion->getLoopInfo()) {
            for (auto& it : parentRegion->getLoopInfo()->reductionVars) {
                string varName = it.first;
                if (it.first.compare(var.first) == 0) {
                    isReductionVar = true;
                    break;
                }
            }
        }

        if (isReductionVar)
            continue;

        string varName = var.first;
        string varType = get<0>(var.second);

        string varArray = "";
        varArraysToChange.push_back("");

        OMP_VARSCOPE varScope = get<1>(var.second);

        bool isStatic = false;
        VarTableTuple_t varTuple;
        bool found = parentNode->getDFG()->getVarTupleFromTable(parentNode, var.first, varTuple);
        if (found) {
            if (get<2>(varTuple).compare("static") == 0)
                isStatic = true;
            varArray = get<6>(varTuple);
        }

        string prefixStr = "";
        string postfixStr = "";
        int parentID = parentNode->getID();

        if (check_isa<OMPTaskDirective>(parentNode->getStmt())
            && parentNode->getTaskInfo()->encapsulateTaskData) {
            prefixStr = "this->taskInputs->";
            postfixStr = "";
            parentID = parentRegion->getID();

            for (auto taskVar : parentNode->ompInputs) {
                string taskVarName = taskVar.first;
                if (taskVarName.compare(varName) == 0) {
                    varScope = get<1>(taskVar.second);
                    break;
                }
            }
        } else if (isStatic == false) {
            if (DARTS_BACKEND) {
                prefixStr = DARTS_PREFIXSTR_DEFAULT;
                if (isSingleThreaded == false)
                    postfixStr = "[this->getID()]";
            } else if (SWARM_BACKEND) {
                prefixStr = "";
                if (isSingleThreaded == false)
                    postfixStr = "[codeletID]";
            }
        }

        if (varScope == OMP_SHARED) {
            ostringstream varNameStream;
            varNameStream << prefixStr << var.first << suffixLang << parentID;
            string varName = varNameStream.str();
            bool found = this->checkIfVarisUsedInLoop(varName, forStmtString, forBodyString);
            if (found) {
                varNamesToDeclare.push_back(var.first);
                varNamesToChange.push_back(varNameStream.str());
#if 0
				varType += "**";
#else
                varType += "*";
#endif
                varTypesToChange.push_back(varType);
                dereferenceSymbol.push_back("OMP_SHARED");
            } else {
                varNamesToDeclare.push_back("NONE");
                varNamesToChange.push_back("NONE");
                varTypesToChange.push_back("NONE");
                dereferenceSymbol.push_back("NONE");
            }
        } else if (varScope == OMP_SHARED_PRIVATE) {
            ostringstream varNameStream;
            varNameStream << prefixStr << var.first << suffixLang << parentID << postfixStr;
            string varName = varNameStream.str();
            bool found = this->checkIfVarisUsedInLoop(varName, forStmtString, forBodyString);
            if (found) {
                varNamesToDeclare.push_back(var.first);
                varNamesToChange.push_back(varNameStream.str());
                varType += "**";
                varTypesToChange.push_back(varType);
                dereferenceSymbol.push_back("OMP_SHARED_PRIVATE");
            } else {
                varNamesToDeclare.push_back("NONE");
                varNamesToChange.push_back("NONE");
                varTypesToChange.push_back("NONE");
                dereferenceSymbol.push_back("NONE");
            }
        } else if (varScope == OMP_PRIVATE) {
            ostringstream varNameStream;
            varNameStream << prefixStr << var.first << suffixLang << parentID << postfixStr;
            string varName = varNameStream.str();
            bool found = this->checkIfVarisUsedInLoop(varName, forStmtString, forBodyString);
            if (found) {
                varNamesToChange.push_back(varNameStream.str());
                varNamesToDeclare.push_back(var.first);
                varType += "*";
                if (varArray.compare("") != 0)
                    varType += "*";
                varTypesToChange.push_back(varType);
                dereferenceSymbol.push_back("OMP_PRIVATE");
            } else {
                varNamesToDeclare.push_back("NONE");
                varNamesToChange.push_back("NONE");
                varTypesToChange.push_back("NONE");
                dereferenceSymbol.push_back("NONE");
            }
        } else if (varScope == OMP_FIRSTPRIVATE) {
            ostringstream varNameStream;
            varNameStream << prefixStr << var.first << suffixLang << parentID << postfixStr;
            string varName = varNameStream.str();
            bool found = this->checkIfVarisUsedInLoop(varName, forStmtString, forBodyString);
            if (found) {
                varNamesToChange.push_back(varNameStream.str());
                varNamesToDeclare.push_back(var.first);
#if 0
				if (varArray.compare("") != 0)
					ptrsStream << varType << " *" << var.first << "_outer"
						<< parentID << "_ptr = "
						<< prefixStr << var.first << "_outer"
						<< parentID << "_ptr;\n";
#endif
                varType += "*";
                if (varArray.compare("") != 0)
                    varType += "*";
                varTypesToChange.push_back(varType);
                dereferenceSymbol.push_back("OMP_FIRSTPRIVATE");
            } else {
                varNamesToDeclare.push_back("NONE");
                varNamesToChange.push_back("NONE");
                varTypesToChange.push_back("NONE");
                dereferenceSymbol.push_back("NONE");
            }
        } else if (varScope == OMP_LASTPRIVATE) {
            ostringstream varNameStream;
            varNameStream << prefixStr << var.first << suffixLang << parentID << postfixStr;
            string varName = varNameStream.str();
            bool found = this->checkIfVarisUsedInLoop(varName, forStmtString, forBodyString);
            if (found) {
                varNamesToDeclare.push_back(var.first);
                varNamesToChange.push_back(varNameStream.str());
                if (varArray.compare("") == 0) {
                    ptrsStream << varType << " *" << var.first << "_outer" << parentNode->getID()
                               << "_ptr = " << prefixStr << var.first << "_outer"
                               << parentNode->getID() << "_ptr;\n";
                } else {
                    ptrsStream << varType << " " << var.first << "_outer" << parentNode->getID()
                               << "_size = " << prefixStr << var.first << "_outer"
                               << parentNode->getID() << "_size;\n";
                }
                varType += "*";
                if (varArray.compare("") != 0)
                    varType += "*";
                varTypesToChange.push_back(varType);
                dereferenceSymbol.push_back("OMP_LASTPRIVATE");
            } else {
                varNamesToDeclare.push_back("NONE");
                varNamesToChange.push_back("NONE");
                varTypesToChange.push_back("NONE");
                dereferenceSymbol.push_back("NONE");
            }
        }
    }
    for (auto var : parentRegion->getOMPVariables()) {
        /*Skip variable declared locally */
        if (get<5>(var.second))
            continue;

        string varName = var.first;
        bool isStatic = false;
        string varType = get<0>(var.second);
        string varArray = get<4>(var.second);
        VarTableTuple_t varTuple;
        bool found = parentNode->getDFG()->getVarTupleFromTable(parentNode, var.first, varTuple);
        if (found) {
            if (get<2>(varTuple).compare("static") == 0)
                isStatic = true;
        }
        string prefixStr = "";
        string postfixStr = "";
        int parentID = parentNode->getID();
        if (check_isa<OMPTaskDirective>(parentNode->getStmt())
            && parentNode->getTaskInfo()->encapsulateTaskData) {
            prefixStr = "this->taskInputs->";
            postfixStr = "";
            parentID = parentRegion->getID();
        } else if (isStatic == false) {
            if (DARTS_BACKEND) {
                prefixStr = DARTS_PREFIXSTR_DEFAULT;
                if (isSingleThreaded == false)
                    postfixStr = "[this->getID()]";
            } else if (SWARM_BACKEND) {
                prefixStr = "";
                if (isSingleThreaded == false)
                    postfixStr = "[codeletID]";
            }
        }

        ostringstream varNameStream;
        varNameStream << prefixStr << varName << suffixLang << parentID << postfixStr;
        string tempVarName = varNameStream.str();
        found = this->checkIfVarisUsedInLoop(tempVarName, forStmtString, forBodyString);
        if (found == false) {
            varNamesToDeclare.push_back("NONE");
            varNamesToChange.push_back("NONE");
            varTypesToChange.push_back("NONE");
            varArraysToChange.push_back("");
            dereferenceSymbol.push_back("NONE");
        } else {
            /*This is to declare pointers to arrays in the form of
            int (*x_ptr[SIZE]) = &x;
            We don't need this for inputs since we use malloc for them.*/
            if (varArray.compare("") != 0) {
                /*Only create pointers for dynamically-allocated arrays*/
                if (varArray.find_first_not_of("[0123456789]") == string::npos) {
                    varArraysToChange.push_back(varArray);
                    varNamesToChange.push_back(varNameStream.str());
                    varNamesToDeclare.push_back(varName);
                    varTypesToChange.push_back(varType);
                    dereferenceSymbol.push_back("PRIVATE");
                } else {
                    varArraysToChange.push_back("");
                    varNamesToChange.push_back("NONE");
                    varNamesToDeclare.push_back("NONE");
                    varTypesToChange.push_back("NONE");
                    dereferenceSymbol.push_back("NONE");
                }
            } else {
                varArraysToChange.push_back("");
                varNamesToChange.push_back(varNameStream.str());
                varNamesToDeclare.push_back(varName);
                varType += "*";
                varTypesToChange.push_back(varType);
                dereferenceSymbol.push_back("PRIVATE");
            }
        }
    }

#if 0
	/*Create the string for the loop's local variable*/
	this->localLoopVarStr = this->loopVar;
	this->localLoopVarStr += suffixLang;
	this->localLoopVarStr += "_counter_temp";
	this->localLoopVarStr += to_string(parentRegion->getID());
#endif

    for (size_t i = 0; i < varNamesToChange.size(); i++) {
        string tempNewName = varNamesToDeclare[i];
        if (tempNewName.compare("NONE") == 0)
            continue;

        string tempName = varNamesToChange[i];
        string tempType = varTypesToChange[i];
        string tempArray = varArraysToChange[i];
        string dereference = dereferenceSymbol[i];

        /*Use the localID instead of the global one obtained with getID()
         * This is mainly for balanced OMPFor regions, where localID is different
         * to the global one. Under any other circumstances, both are the same*/
        string toBeReplaced = "this->getID()";
        size_t pos = tempName.find(toBeReplaced);
        if (pos != string::npos)
            tempName.replace(pos, toBeReplaced.length(), "this->getLocalID()");

/*Replace the transformed loop's var in the code with the local var*/
#if 0
		if(varNamesToChange[i].find(this->transformedLoopVarStr) != std::string::npos ||
		   this->transformedLoopVarStr.find(varNamesToChange[i]) != std::string::npos){
			varNamesToDeclare[i] = localLoopVarStr;
			continue;
		}
#endif
        if (tempArray.compare("") == 0)
            ptrsStream << tempType << " " << tempNewName << " = ";
        else
            ptrsStream << tempType << "(*" << tempNewName << ")" << tempArray << " = ";

        if (dereference.compare("OMP_SHARED") != 0)
            ptrsStream << "&";

        ptrsStream << "(" << tempName << ");\n"
                   << "(void)" << tempNewName << "/*" << dereference << "*/"
                   << ";\n";

#if 0
		if(varNamesToChange[i].find(this->transformedLoopVarStr) != std::string::npos ||
		   this->transformedLoopVarStr.find(varNamesToChange[i]) != std::string::npos){
			varNamesToDeclare[i] = localLoopVarStr;
		}
#endif
    }
}

void LoopInfo::updateLastPrivateVars(
    omp2cd_space::TPRegion* parentRegion, DFGNode* parentNode, std::ostringstream& outputStream)
{
    outputStream << "/*Update lastprivate variables, if any*/\n";
    if (changePtrs && parentRegion->singleThreaded == false) {
        for (auto var : parentRegion->getOMPInputs()) {
            if (get<1>(var.second) == OMP_LASTPRIVATE) {

                VarTableTuple_t varTuple;
                bool found
                    = parentNode->getDFG()->getVarTupleFromTable(parentNode, var.first, varTuple);
                if (found && get<4>(varTuple) == 0) {
                    string varArray = get<6>(varTuple);
                    string varSize = get<8>(varTuple);

                    string prefix = "myTP->";
                    string postfix = "[this->getLocalID()]";
                    string localIncrStr
                        = parentNode->getLoopInfo()->getForStmtIncr(parentNode, prefix, postfix);
                    outputStream << localIncrStr << ";\n";

                    if (varArray.compare("") == 0) {
                        outputStream << "*(" << var.first << "_outer" << parentRegion->getID()
                                     << "_ptr) = *" << var.first << ";\n";
                    } else {
#if 0
						std::string varSizeStr;
						parentNode->getVarSize(varSize, parentNode,
									   varSizeStr,
									   DARTS_PREFIXSTR_DEFAULT,
									   "[this->getLocalID()]");
#endif
                        std::ostringstream varSizeStr;
                        varSizeStr << var.first << "_outer" << parentNode->getID() << "_size";

                        outputStream
                            << "for (uint64_t k_outer" << parentNode->getID() << " = 0;"
                            << " k_outer" << parentNode->getID() << " < " << varSizeStr.str()
                            << "; k_outer" << parentNode->getID() << "++){\n"
                            << "temp_" << var.first << "_outer_ptr[k_outer" << parentNode->getID()
                            << "] = "
                            << "temp_" << var.first << "_darts_ptr[k_outer" << parentNode->getID()
                            << "];\n"
                            << "}\n";
                    }
                }
            }
        }
    }
}

bool LoopInfo::checkIfVarisUsedInLoop(
    std::string& varName, std::string& forStmtString, std::string& forBodyString)
{
    bool found = false;
    size_t newPos = forStmtString.find(varName);
    if (newPos != string::npos)
        found = true;

    if (found == false) {
        newPos = forBodyString.find(varName);
        if (newPos != string::npos)
            found = true;
    }

    return found;
}

void LoopInfo::replaceVarsForPtrs(std::vector<std::string>& varNamesToDeclare,
    std::vector<std::string>& varNamesToChange, std::vector<std::string>& dereferenceSymbol,
    DFGNode* node, std::ostringstream& changedStream, bool useLocalLoopVar)
{
    string fireCode = changedStream.str();
    for (size_t i = 0; i < varNamesToChange.size(); i++) {
        if (varNamesToDeclare[i].compare("NONE") == 0)
            continue;
        size_t newPos = 0;
        do {
            string tempName = varNamesToChange[i];
            string tempNewName = "";
            if (dereferenceSymbol[i].compare("OMP_SHARED") != 0)
                tempNewName += "*";
            tempNewName += varNamesToDeclare[i];
            /*Use the local loop var instead of the loop var's pointer for
            increased performance.*/
            if (useLocalLoopVar) {
                if (tempName.find(this->transformedLoopVarStr) != std::string::npos
                    || this->transformedLoopVarStr.find(tempName) != std::string::npos) {
                    tempNewName = this->localLoopVarStr;
                }
            }
            newPos = fireCode.find(tempName);
            if (newPos != string::npos) {
                fireCode.replace(newPos, tempName.length(), tempNewName);
                // newPos = newPos + tempName.length();
                newPos = newPos + tempNewName.length();
            }
        } while ((newPos != string::npos) && (newPos < fireCode.length()));
    }
    changedStream.str("");
    changedStream.clear();
    changedStream << fireCode;
}

void LoopInfo::replaceLoopVarForLocal(DFGNode* node, std::ostringstream& changedStream)
{
    /*Use the local loop var instead of the loop var's pointer for
    increased performance.*/
    string fireCode = changedStream.str();
    size_t newPos = 0;
    do {
        string tempName = this->transformedLoopVarStr;
        string tempNewName = this->localLoopVarStr;
        newPos = fireCode.find(tempName);
        if (newPos != string::npos) {
            fireCode.replace(newPos, tempName.length(), tempNewName);
            newPos = newPos + tempNewName.length();
        }

    } while ((newPos != string::npos) && (newPos < fireCode.length()));
    changedStream.str("");
    changedStream.clear();
    changedStream << fireCode;
}

void LoopInfo::createPrologue(std::vector<std::string>& varNamesToDeclare,
    std::vector<std::string>& varNamesToChange, std::vector<std::string>& varTypesToChange,
    std::vector<std::string>& dereferenceSymbol, omp2cd_space::TPRegion* parentRegion,
    DFGNode* parentNode, DFGNode* node, std::ostringstream& prologueStream,
    std::string& fireStmtStream, std::string& fireCodeStream)
{
    /*Change pointers in a for loop */
    if (changePtrs && parentRegion->singleThreaded == false)
        this->findVarsToChange(varNamesToDeclare, varNamesToChange, varTypesToChange,
            dereferenceSymbol, parentRegion, parentNode, prologueStream, fireStmtStream,
            fireCodeStream);

    if (parentRegion->isInlinedRegion())
        return;

    bool atLeastOneLastPrivate = false;
    for (auto var : parentRegion->mainNode->ompInputs) {
        if (get<1>(var.second) == OMP_LASTPRIVATE) {
            atLeastOneLastPrivate = true;
            break;
        }
    }

    if (atLeastOneLastPrivate)
        prologueStream << "bool containsLastIteration = false;\n";

    if (DARTS_BACKEND) {
        prologueStream << "bool isThereNewIteration = " << DARTS_PREFIXSTR_DEFAULT
                       << "requestNewRangeIterations" << parentRegion->getID() << "(("
                       << this->loopVarType << "*)"
                       << "&(this->endRange), this->getLocalID()";

        if (atLeastOneLastPrivate)
            prologueStream << ", &containsLastIteration";

        prologueStream << ");";
    } else if (SWARM_BACKEND) {

        prologueStream << this->loopVarType << " startRange = 0, endRange = 0;\n";

        prologueStream << "bool isThereNewIteration = "
                       << "requestNewRangeIterations" << parentRegion->getID() << "(("
                       << this->loopVarType << "*)&(startRange),"
                       << "(" << this->loopVarType << "*)&(endRange),"
                       << "codeletID";

        if (atLeastOneLastPrivate)
            prologueStream << ", &containsLastIteration";

        prologueStream << ");";
    }

    /*If we don't have iterations to perform, signal the next codelet or
       update the global reduction vars if we have to. */
    prologueStream << "if(isThereNewIteration == false)\n"
                   << "{\n";
    if (this->reductionVars.size() > 0) {
        prologueStream << "goto updateReduceVars;\n";
    } else {
        node->signalNext(prologueStream);
        prologueStream << "return;\n";
    }
    prologueStream << "}\n";
}

void LoopInfo::createForStmt(
    omp2cd_space::TPRegion* parentRegion, std::ostringstream& fireStmtStream)
{
    string localLoopVarInit = this->loopVarType;
    localLoopVarInit += " ";
    localLoopVarInit += this->localLoopVarStr;
    localLoopVarInit += " = ";

    if (parentRegion->isInlinedRegion() == false) {
        if (DARTS_BACKEND)
            localLoopVarInit += this->transformedLoopVarStr;
        else if (SWARM_BACKEND)
            localLoopVarInit += "startRange";
    } else {
        string loopInit
            = this->getForStmtInit(parentRegion->getMainNode(), "myTP->", "[this->getID()]");
        if (DARTS_BACKEND)
            localLoopVarInit += loopInit;
    }

/*Use the localID instead of the global one obtained with getID()
This is mainly for balanced OMPFor regions, where localID is different
to the global one. Under any other circumstances, both are the same*/
#if 0
	string toBeReplaced = "this->getID()";
	size_t pos = localLoopVarInit.find(toBeReplaced);
		if (pos != string::npos)
			localLoopVarInit.replace(pos, toBeReplaced.length(), "this->getLocalID()");
#endif

    /*Write the local loop var in the increment string*/
    string localLoopVarIncr = this->incrStr;
    size_t pos = localLoopVarIncr.find(this->transformedLoopVarStr);
    while (pos != string::npos) {
        localLoopVarIncr.replace(pos, this->transformedLoopVarStr.length(), this->localLoopVarStr);
        pos = localLoopVarIncr.find(this->transformedLoopVarStr);
    }

    if (parentRegion->isInlinedRegion() == false) {
        fireStmtStream << "for (" << localLoopVarInit << ";" << this->localLoopVarStr
                       << this->loopCondOpcode << "endRange && " << this->localLoopVarStr
                       << this->loopCondOpcode;

        if (DARTS_BACKEND)
            fireStmtStream << DARTS_PREFIXSTR_DEFAULT;

        fireStmtStream << "lastIteration" << parentRegion->getID() << ";" << localLoopVarIncr
                       << ")\n";
    } else {
        string loopLimit
            = this->getForStmtLimit(parentRegion->getMainNode(), "myTP->", "[this->getID()]");
        fireStmtStream << "for (" << localLoopVarInit << ";" << this->localLoopVarStr
                       << this->loopCondOpcode << loopLimit << ";" << localLoopVarIncr << ")\n";
    }
}

void LoopInfo::printFireCode(std::ostringstream& outputStream, TPRegion* parentRegion,
    DFGNode* node, DFGNode* parentNode, bool atLeastOneLastPrivate)
{
    vector<string> varNamesToDeclare;
    vector<string> varNamesToChange;
    vector<string> varTypesToChange;
    vector<string> dereferenceSymbol;

    string loopCondOpcode = this->loopCondOpcode;
    size_t posEqual = loopCondOpcode.find("=");
    if (posEqual != string::npos)
        loopCondOpcode.replace(posEqual, 1, " ");

    /*Create the for stmt */
    ostringstream fireStmtStream;
    this->createForStmt(parentRegion, fireStmtStream);

    /*Create the for body */
    ostringstream fireCodeStream;
    fireCodeStream << node->fireCodeStr;

    /*The prologue: Instructions needed before the for */
    std::ostringstream prologueStream;
    node->printReductionVarPtrDefs(prologueStream, parentRegion);
    string stmtString = fireStmtStream.str();
    string codeString = fireCodeStream.str();
    this->createPrologue(varNamesToDeclare, varNamesToChange, varTypesToChange, dereferenceSymbol,
        parentRegion, parentNode, node, prologueStream, stmtString, codeString);

    /*use ptrs instead of vars in the for stmt*/
    this->replaceVarsForPtrs(
        varNamesToDeclare, varNamesToChange, dereferenceSymbol, node, fireStmtStream, false);

    /*use ptrs instead of vars in the for body*/
    this->replaceVarsForPtrs(
        varNamesToDeclare, varNamesToChange, dereferenceSymbol, node, fireCodeStream);

    /*Arrage everything in the output stream */
    outputStream << prologueStream.str();

    if (parentRegion->isInlinedRegion() == false) {
        if (get<0>(this->schedulingPolicy) != STATIC_SCHED)
            outputStream << "while(isThereNewIteration)\n{\n";

        if (atLeastOneLastPrivate) {
            outputStream << "if(containsLastIteration){\n"
                         << fireStmtStream.str() << "{\n"
                         << fireCodeStream.str();

            this->updateLastPrivateVars(parentRegion, parentNode, outputStream);

            outputStream << "}\n"
                         << "} else {\n";
        }
    }

    outputStream << fireStmtStream.str() << "{\n" << fireCodeStream.str() << "}\n";

    if (parentRegion->isInlinedRegion() == false) {

        if (atLeastOneLastPrivate)
            outputStream << "}\n";

        if (get<0>(this->schedulingPolicy) != STATIC_SCHED) {
            if (get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {

                if (DARTS_BACKEND) {
                    outputStream << "/*dont ask for more iterations if the scheduling "
                                 << "selected at runtime is static*/\n"
                                 << "if(" << DARTS_PREFIXSTR_DEFAULT << "ompLoopSched"
                                 << parentRegion->getID() << " == 1)\n"
                                 << "break;\n";
                } else if (SWARM_BACKEND) {
                    outputStream << "/*dont ask for more iterations if the scheduling "
                                 << "selected at runtime is static*/\n"
                                 << "if(ompLoopSched" << parentRegion->getID() << " == 1)\n"
                                 << "break;\n";
                }
            }

            if (DARTS_BACKEND) {
                outputStream << "isThereNewIteration = " << DARTS_PREFIXSTR_DEFAULT
                             << "requestNewRangeIterations" << parentRegion->getID()
                             << "(&(this->endRange), this->getLocalID()";

                if (atLeastOneLastPrivate)
                    outputStream << ", &containsLastIteration";

                outputStream << ");\n";
            } else if (SWARM_BACKEND) {
                outputStream << "isThereNewIteration = "
                             << "requestNewRangeIterations" << parentRegion->getID()
                             << "(&startRange, &endRange, codeletID";

                if (atLeastOneLastPrivate)
                    outputStream << ", &containsLastIteration";

                outputStream << ");\n";
            }

            outputStream << "}\n";
        }

        node->replaceReductionVarInFireCode(outputStream, parentRegion);

        /*Update the reduction variables if any */
        if (parentRegion->getLoopInfo()->reductionVars.size() > 0)
            node->updateGlobalReductionVars(outputStream, parentRegion);
    }
}

void LoopInfo::printTPLoopFireCode(
    std::ostringstream& outputStream, TPRegion* parentRegion, DFGNode* node)
{
    ostringstream fireCodeStream;

    if (get<0>(this->schedulingPolicy) != STATIC_SCHED)
        fireCodeStream << "while(isThereNewIteration)\n{\n";

    outputStream << fireCodeStream.str();

    /*Create a string with the original code. */
    if (check_isa<ForStmt>(node->getStmt()) && node->myNodes.size() > 0) {
        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;
        Policy.darts_printingInTPLoop = true;
        Policy.darts_TPInputs = parentRegion->getOMPInputs();
        Policy.darts_TPVars = parentRegion->getOMPVariables();
        Policy.backEnd = backEnd;

        /*Print any instructions before the omp for, if any */
        if (parentRegion->prev) {
            for (DFGNode*& toPrintNode : parentRegion->prev->myNodes) {
                string ostreamString;
                raw_string_ostream rawStringOStream(ostreamString);
                toPrintNode->getStmt()->printPretty(rawStringOStream, nullptr, Policy);
                outputStream << rawStringOStream.str();
            }
        }

        /*Print the omp for */

        /*This compound stmt is to make sure the declaration of the loop
         * var in the for() does not interfere with a previoius decl*/
        outputStream << "{\n";

        outputStream << "for(" << this->loopVarType << " " << this->loopVar << " = startRange; "
                     << this->loopVar << " < "
                     << " endRange;"
                     << " " << this->getForStmtIncrRaw() << ")"
                     << "{\n";

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        node->myNodes.front()->getStmt()->printPretty(rawStringOStream, nullptr, Policy);
        outputStream << rawStringOStream.str() << "}\n"
                     << "}\n";

        /*Print any instructions after the omp for, if any */
        if (parentRegion->next) {
            for (DFGNode*& toPrintNode : parentRegion->next->myNodes) {
                string ostreamString;
                raw_string_ostream rawStringOStream(ostreamString);
                toPrintNode->getStmt()->printPretty(rawStringOStream, nullptr, Policy);
                if (check_isa<Expr>(toPrintNode->getStmt()) && !rawStringOStream.str().empty()) {
                    rawStringOStream << ";\n";
                }
                outputStream << rawStringOStream.str();
            }
        }
    }
}

void LoopInfo::printLoopParamInitialization(std::ostringstream& outputStream, TPRegion* region)
{
    int loopID = region->getID();

    if (get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
        outputStream << "ompLoopSched" << loopID << " = "
                     << "ompSchedulePolicy;\n"
                     << "ompLoopChunk" << loopID << " = "
                     << "ompScheduleChunk;\n";
    }

    if (get<0>(this->schedulingPolicy) == STATIC_SCHED
        || get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
        outputStream << "range" << loopID << " = "
                     << "abs (lastIteration" << loopID << " - initIteration" << loopID << ") / "
                     << this->incrVal << ";\n"
                     << "rangePerCodelet" << loopID << " = ";

        if (DARTS_BACKEND)
            outputStream << "range" << loopID << " / numThreads;\n";
        else if (SWARM_BACKEND)
            outputStream << "range" << loopID << " / numThreads" << loopID << ";\n";

        outputStream << "minIteration" << loopID << " = "
                     << "min";

        if (DARTS_BACKEND)
            outputStream << "<" << this->loopVarType << ">";

        outputStream << "(lastIteration" << loopID << ", initIteration" << loopID << ");\n"
                     << "remainderRange" << loopID << " = ";

        if (DARTS_BACKEND)
            outputStream << "range" << loopID << " % numThreads;\n";
        else if (SWARM_BACKEND)
            outputStream << "range" << loopID << " % numThreads" << loopID << ";\n";
    }

    if (get<0>(this->schedulingPolicy) == DYNAMIC_SCHED
        || get<0>(this->schedulingPolicy) == RUNTIME_SCHED) {
        outputStream << "nextIteration" << loopID << " = initIteration" << loopID << ";\n";
    }
}
}
