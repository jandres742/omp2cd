#include "include/Compiler/ProcessVisitor.h"
#include "include/DFG/AtomicNode.h"
#include "include/DFG/BranchInfo.h"
#include "include/DFG/CopyPrivateClause.h"
#include "include/DFG/CopyinClause.h"
#include "include/DFG/FunctionInfo.h"
#include "include/DFG/LoopCondNode.h"
#include "include/DFG/LoopInfo.h"
#include "include/DFG/TaskInfo.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

bool ompASTVisitor::TraverseDecl(Decl* decl)
{
    DFGNode* previousNode;

    if (isa<FunctionDecl>(decl)) {

        /*Clear the task scope we are keeping for the depend clause.*/
        this->clearDependTaskNodes();

        FunctionDecl* func = dyn_cast<FunctionDecl>(decl);

        string funcName = getNameFromDecl(func);

        FunctionTable_t::iterator it = functionTable.find(funcName);

        int nodeID = -1;
        if (it != functionTable.end())
            nodeID = get<2>(it->second);
        else
            nodeID = this->getNewNodeID();

        DFGNode* tempNode = new DFGNode(func, this->currentNode, nodeID);
        this->currentNode->addChild(tempNode);
        previousNode = this->currentNode;
        this->currentNode = tempNode;

        /*Add the function declaration which may or may not have a body.*/
        this->myDFG.addFunctionDecl(this->currentNode);

    } else if (isa<TagDecl>(decl)) {
        TagDecl* tagDecl = dyn_cast<TagDecl>(decl);
        if (tagDecl && tagDecl->isEnum())
            constantsDeclStream << "enum " << tagDecl->getNameAsString() << "{\n";
    }

    RecursiveASTVisitor::TraverseDecl(decl);

    if (isa<FunctionDecl>(decl)) {
        /*Add the function declaration associated with its body.*/
        if (this->currentNode->myNodes.size() > 0)
            this->myDFG.addFunction(this->currentNode);
        this->currentNode = previousNode;
    } else if (isa<TagDecl>(decl)) {
        TagDecl* tagDecl = dyn_cast<TagDecl>(decl);
        if (tagDecl && tagDecl->isEnum()) {
            constantsDeclStream << "};\n";
            this->myDFG.numEnumTypesPrinted = 0;
        }
    }

    return true;
}

bool ompASTVisitor::TraverseStmt(Stmt* s)
{
    DFGNode* previousNode;

    bool newNodeCreated = false;

    /*Dont create nodes in the ast for the copyin clause, which are added
       automatically by clang with the stmt copyin.dst = copyin.src. we do
       this operation in the constructor of the corresponding OMP region. */
    /*Do the same for reduction nodes with stmts in the form of
       .reduction.lhs += .reduction.lhs. ... */
    if (check_isa<BinaryOperator>(s)) {
        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        PrintingPolicy Policy = context->getPrintingPolicy();
        s->printPretty(rawStringOStream, nullptr, Policy);
        if (rawStringOStream.str().find(".copyin") != string::npos
            || rawStringOStream.str().find(".reduction.") != string::npos)
            return true;
    }

    if (check_isa<IfStmt>(s)) {
        DFGNode* tempNode = new DFGNode(s, this->currentNode, this->getNewNodeID());
        this->currentNode->addChild(tempNode);
        previousNode = this->currentNode;
        this->currentNode = tempNode;

        IfStmt* ifStmt = dyn_cast<IfStmt>(s);

        newNodeCreated = true;

        this->VisitIfStmt(ifStmt);
    } else if (check_isa<ForStmt>(s)) {
        DFGNode* tempNode = new DFGNode(s, this->currentNode, this->getNewNodeID());
        this->currentNode->addChild(tempNode);
        previousNode = this->currentNode;
        this->currentNode = tempNode;

        ForStmt* forStmt = dyn_cast<ForStmt>(s);

        newNodeCreated = true;

        this->VisitForStmt(forStmt);
    } else if (check_isa<WhileStmt>(s)) {
        DFGNode* tempNode = new DFGNode(s, this->currentNode, this->getNewNodeID());
        this->currentNode->addChild(tempNode);
        previousNode = this->currentNode;
        this->currentNode = tempNode;

        WhileStmt* whileStmt = dyn_cast<WhileStmt>(s);

        newNodeCreated = true;

        this->VisitWhileStmt(whileStmt);
    } else if (check_isa<DoStmt>(s)) {
        DFGNode* tempNode = new DFGNode(s, this->currentNode, this->getNewNodeID());
        this->currentNode->addChild(tempNode);
        previousNode = this->currentNode;
        this->currentNode = tempNode;

        DoStmt* doStmt = dyn_cast<DoStmt>(s);

        newNodeCreated = true;

        this->VisitDoStmt(doStmt);
    } else if (this->isTraversableStmt(s)) {
        /*Ignore omp flush directives since threads are automatically written in the TP frame.*/
        if (check_isa<OMPFlushDirective>(s)) {
            return true;
        }

        /*Avoid creating a node for a copyprivate clause: For some reason, it's in the AST */
        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        PrintingPolicy Policy = context->getPrintingPolicy();
        s->printPretty(rawStringOStream, nullptr, Policy);
        if (rawStringOStream.str().find(".copyprivate.dst") == string::npos) {
            newNodeCreated = true;
            DFGNode* tempNode = new DFGNode(s, this->currentNode, this->getNewNodeID());
            this->currentNode->addChild(tempNode);
            previousNode = this->currentNode;
            this->currentNode = tempNode;
        }
    }

    if (s && (isa<IfStmt>(s) || isa<ForStmt>(s) || isa<WhileStmt>(s) || isa<DoStmt>(s)) == false) {
        RecursiveASTVisitor::TraverseStmt(s);
    }

    if (check_isa<IfStmt>(s)) {
        /* an if node should have at most two child nodes, the if branch and the else branch.
        nested if stmts are handled as if-else(if-else). Since we may add nodes afterwards,
        let's store the those nodes to not mess with the control flow*/

        if (this->currentNode->myNodes.size() > 1) {
            DFGNode* elseBodyNode = this->currentNode->myNodes.front()->next;
            if (check_isa<CompoundStmt>(elseBodyNode->getStmt())) {
                elseBodyNode = elseBodyNode->myNodes.front();
            } else {
                uint8_t* compoundStmtPtr = (uint8_t*)this->context->Allocate(sizeof(CompoundStmt));
                CompoundStmt* compoundStmt = nullptr;
                if (compoundStmtPtr) {
                    compoundStmt = new (compoundStmtPtr)
                        CompoundStmt(elseBodyNode->getStmt()->getLocStart());
                    DFGNode* newNode = new DFGNode(compoundStmt, this->currentNode, getNewNodeID());
                    if (newNode) {
                        this->currentNode->myNodes.swap(elseBodyNode, newNode);
                        elseBodyNode->parent = newNode;
                        newNode->myNodes.insert(elseBodyNode);
                        elseBodyNode = newNode;
                    }
                }
            }
            this->currentNode->getBranchInfo()->elseBodyNode = elseBodyNode;
        }

        if (this->currentNode->myNodes.size() > 0) {
            DFGNode* ifBodyNode = this->currentNode->myNodes.front();
            if (check_isa<CompoundStmt>(ifBodyNode->getStmt())) {
                ifBodyNode = ifBodyNode->myNodes.front();
            } else {
                uint8_t* compoundStmtPtr = (uint8_t*)this->context->Allocate(sizeof(CompoundStmt));
                CompoundStmt* compoundStmt = nullptr;
                if (compoundStmtPtr) {
                    compoundStmt
                        = new (compoundStmtPtr) CompoundStmt(ifBodyNode->getStmt()->getLocStart());
                    DFGNode* newNode = new DFGNode(compoundStmt, this->currentNode, getNewNodeID());
                    if (newNode) {
                        this->currentNode->myNodes.swap(ifBodyNode, newNode);
                        ifBodyNode->prev = nullptr;
                        ifBodyNode->next = nullptr;
                        ifBodyNode->parent = newNode;
                        newNode->myNodes.insert(ifBodyNode);
                        ifBodyNode = newNode;
                    }
                }
            }
            this->currentNode->getBranchInfo()->ifBodyNode = ifBodyNode;
        }

    } else if (this->currentNode && check_isa<ForStmt>(this->currentNode->s)
        && this->currentNode->myLoopInfo) {
        ForStmt* forStmt = dyn_cast<ForStmt>(this->currentNode->getStmt());
        this->currentNode->myLoopInfo->initForStmtInfo(forStmt, this->currentNode);
    } else if (this->currentNode && check_isa<WhileStmt>(this->currentNode->getStmt())
        && this->currentNode->myLoopInfo) {
        WhileStmt* whileStmt = dyn_cast<WhileStmt>(this->currentNode->getStmt());
        this->currentNode->myLoopInfo->initWhileStmtInfo(whileStmt, this->currentNode);
    } else if (this->currentNode && check_isa<DoStmt>(this->currentNode->getStmt())
        && this->currentNode->myLoopInfo) {
        DoStmt* doStmt = dyn_cast<DoStmt>(this->currentNode->getStmt());
        this->currentNode->myLoopInfo->initDoStmtInfo(doStmt, this->currentNode);
    }
    /*Check if any of the children of the current node is complex (has an omp directive or a
     * function call) */
    if ((check_isa<ForStmt>(this->currentNode->getStmt())
            && (this->currentNode->parent && (this->currentNode->parent->isOMPFor()) == false))
        || (check_isa<WhileStmt>(this->currentNode->getStmt()))
        || (check_isa<DoStmt>(this->currentNode->getStmt()))) {
        this->currentNode->checkIfComplexStmt();
    }

    if (newNodeCreated) {
        this->currentNode = previousNode;
    }

    return true;
}

bool ompASTVisitor::VisitDecl(clang::Decl* d) { return true; }

bool ompASTVisitor::VisitTypedefDecl(clang::TypedefDecl* d) { return true; }

bool ompASTVisitor::VisitDeclRefExpr(clang::DeclRefExpr* s)
{
    /* A var used inside an omp region is added on VisitCapturedStmt or from
     * the data-sharing clauses (private, firstprivate, etc.). However, if it
     * is a global variable, an inner omp region might not have it on its
     * input list, even if the enclosing parallel region has it with a clause.
     * So we need to make sure the var is added to the inner region. If the var
     * is not in the enclosing parallel region's input's list, then we just
     * use its original name.
     * */
    string varName = getNameFromDecl(s->getFoundDecl());

    /*Dont add threadprivate vars */
    bool isThreadPrivate = false;
    VarTableTuple_t varTuple;
    bool found = this->myDFG.getVarTupleFromTable(this->currentNode, varName, varTuple);
    if (found)
        isThreadPrivate = get<4>(varTuple);

    DFGNode* ompParentNode = this->currentNode->findOMPParent();

    if (ompParentNode && isThreadPrivate == false) {

        TPInputs_t::iterator inputsIt = ompParentNode->ompInputs.find(varName);

        /* If the omp parent node does not have the var and the ompParentNode
         * is not parallel, find the enclosing parallel node and try to
         * find the var there.
         * */
        if (inputsIt == ompParentNode->ompInputs.end() && ompParentNode->isOMPParallel() == false) {
            DFGNode* ompParallelNode = ompParentNode->findOMPParent();
            if (ompParallelNode) {
                TPInputs_t::iterator inputsParallelIt = ompParallelNode->ompInputs.find(varName);

                /*If found, then make sure the var is in the ompParentNode's
                 * input's list*/
                if (inputsParallelIt != ompParallelNode->ompInputs.end()) {
                    /* Look for the var's type in the var table if empty in the inputs */
                    string varType = get<0>(inputsParallelIt->second);
                    string varArray = get<4>(inputsParallelIt->second);

                    if (varType.empty()) {
                        VarTableTuple_t varTuple;
                        bool found = this->myDFG.getVarTupleFromTable(
                            this->currentNode, varName, varTuple);
                        if (found) {
                            varType = get<0>(varTuple);
                            varArray = get<6>(varTuple);
                        }
                    }
                    /* Add the var to the ompParentNode */
                    OMP_VARINFO newTuple(
                        varType, get<1>(inputsParallelIt->second), -1, "", varArray, false);
                    pair<string, OMP_VARINFO> newInput(varName, newTuple);
                    ompParentNode->ompInputs.insert(newInput);
                }
            }
        }
    }
    return true;
}

bool ompASTVisitor::VisitStmt(Stmt* s)
{
    this->setMyIfNode(this->currentNode);
    return true;
}

bool ompASTVisitor::VisitDoStmt(DoStmt* doStmt)
{
    this->currentNode->myLoopInfo = new LoopInfo(this->context);

    this->setMyIfNode(this->currentNode);

    DFGNode* tempCurrentNode = this->currentNode;

    Expr* condStmt = doStmt->getCond();
    if (condStmt) {
        LoopCondNode* condNode
            = new LoopCondNode(condStmt, this->currentNode, this->getNewNodeID());
        condNode->myLoopInfo = this->currentNode->myLoopInfo;
        this->currentNode->myLoopInfo->condNode = condNode;

        RecursiveASTVisitor::TraverseStmt(condStmt);
    }
    this->currentNode = tempCurrentNode;

    /* 	delete any child node that could've been attached to the while stmt node
    since the only child node should be the body */
    if (this->currentNode->myNodes.size() > 0) {
        this->currentNode->myNodes.clear();
    }

    Stmt* bodyStmt = doStmt->getBody();
    if (bodyStmt) {
        this->TraverseStmt(bodyStmt);
    }

    return true;
}

bool ompASTVisitor::VisitWhileStmt(WhileStmt* whileStmt)
{
    this->currentNode->myLoopInfo = new LoopInfo(this->context);

    this->setMyIfNode(this->currentNode);

    DFGNode* tempCurrentNode = this->currentNode;

    Expr* condStmt = whileStmt->getCond();
    if (condStmt) {
        LoopCondNode* condNode
            = new LoopCondNode(condStmt, this->currentNode, this->getNewNodeID());
        condNode->myLoopInfo = this->currentNode->myLoopInfo;
        this->currentNode->myLoopInfo->condNode = condNode;

        RecursiveASTVisitor::TraverseStmt(condStmt);
    }
    this->currentNode = tempCurrentNode;

    /* delete any child node that could've been attached to the while stmt node
       since the only child node should be the body */
    if (this->currentNode->myNodes.size() > 0) {
        this->currentNode->myNodes.clear();
    }

    Stmt* bodyStmt = whileStmt->getBody();
    if (bodyStmt)
        this->TraverseStmt(bodyStmt);

    return true;
}

bool ompASTVisitor::VisitForStmt(ForStmt* forStmt)
{
    if (this->currentNode->parent->isOMPFor())
        this->currentNode->myLoopInfo = this->currentNode->parent->myLoopInfo;
    else
        this->currentNode->myLoopInfo = new LoopInfo(this->context);

    this->setMyIfNode(this->currentNode);

    DFGNode* tempCurrentNode = this->currentNode;

    Stmt* initStmt = forStmt->getInit();
    if (initStmt) {
        DFGNode* initNode = new DFGNode(initStmt, this->currentNode, this->getNewNodeID());
        initNode->myLoopInfo = this->currentNode->myLoopInfo;
        this->currentNode->myLoopInfo->initNode = initNode;
        RecursiveASTVisitor::TraverseStmt(initStmt);
    }
    this->currentNode = tempCurrentNode;

    Expr* condStmt = forStmt->getCond();
    if (condStmt) {
        LoopCondNode* condNode
            = new LoopCondNode(condStmt, this->currentNode, this->getNewNodeID());
        condNode->myLoopInfo = this->currentNode->myLoopInfo;
        this->currentNode->myLoopInfo->condNode = condNode;

        RecursiveASTVisitor::TraverseStmt(condStmt);
    }
    this->currentNode = tempCurrentNode;

    Expr* incStmt = forStmt->getInc();
    if (incStmt) {
        DFGNode* incNode = new DFGNode(incStmt, this->currentNode, this->getNewNodeID());
        incNode->myLoopInfo = this->currentNode->myLoopInfo;
        this->currentNode->myLoopInfo->incNode = incNode;

        RecursiveASTVisitor::TraverseStmt(incStmt);
    }
    this->currentNode = tempCurrentNode;

    /* delete any child node that could've been attached to the for stmt node
       since the only child node should be the body */
    if (this->currentNode->myNodes.size() > 0) {
        this->currentNode->myNodes.clear();
    }

    Stmt* bodyStmt = forStmt->getBody();
    if (bodyStmt)
        this->TraverseStmt(bodyStmt);

    return true;
}

bool ompASTVisitor::VisitIfStmt(IfStmt* ifStmt)
{
    this->currentNode->myBranchInfo = new BranchInfo(currentNode);

    this->setMyIfNode(this->currentNode);

    Expr* condStmt = ifStmt->getCond();
    if (condStmt) {
        DFGNode* condNode = new DFGNode(condStmt, this->currentNode, this->getNewNodeID());
        condNode->myIfNode = this->currentNode;

        this->currentNode->myBranchInfo->condNode = condNode;
    }

    Stmt* thenStmt = ifStmt->getThen();
    if (thenStmt)
        this->TraverseStmt(thenStmt);

    Stmt* elseStmt = ifStmt->getElse();
    if (elseStmt)
        this->TraverseStmt(elseStmt);

    return true;
}

bool ompASTVisitor::VisitCallExpr(clang::CallExpr* s)
{
    string calleeFunctionName = this->getCalleeFunctionName(s);
    FunctionTable_t::iterator it = functionTable.find(calleeFunctionName);
    if (it != functionTable.end() && get<1>(it->second)) {
        this->currentNode->calleeFunctionNodeName = calleeFunctionName;
        this->currentNode->calleeFunctionNodeType = get<0>(it->second);
        this->currentNode->calleeFunctionNodeID = get<2>(it->second);
        this->currentNode->calleeFunctionNodeIsSingleThreaded = get<4>(it->second);

        DFGNode* ompParentNode = this->currentNode->findOMPParent();
        if (ompParentNode && check_isa<OMPTaskDirective>(ompParentNode->getStmt()))
            ompParentNode->getTaskInfo()->containsFunctionCall = true;
    }
    /*Check if the call is to alloc and mark the variable to be deleted later with free since
    we are replacing alloc calls with malloc*/
    else if (calleeFunctionName.compare("__builtin_alloca") == 0
        || calleeFunctionName.compare("alloca") == 0) {

        DFGNode* parentFunction = this->currentNode->findFunctionParent();

        /*TODO: We will delete vars allocated with alloca() inside functions that have been
        converted to TPs,
        i.e. that do not have parallel regions*/
        FunctionTable_t::iterator it = functionTable.find(parentFunction->getName());
        if (it != functionTable.end() && get<5>(it->second) == 0) {

            this->myDFG.useFunctionStack = true;

            /*Look for the parent node, which should be either a binaryoperator or declstmt*/
            DFGNode* parentStmtNode = this->currentNode->parent;
            bool found = false;
            while (parentStmtNode) {
                if (check_isa<BinaryOperator>(parentStmtNode->getStmt())
                    || check_isa<DeclStmt>(parentStmtNode->getStmt())) {
                    found = true;
                    break;
                }
                parentStmtNode = parentStmtNode->parent;
            }

            if (found) {

                /*Find the function or omp region to which this belongs*/
                DFGNode* parentNode = parentStmtNode->findParentNode();

                if (check_isa<BinaryOperator>(parentStmtNode->getStmt())) {
                    BinaryOperator* binOp = dyn_cast<BinaryOperator>(parentStmtNode->getStmt());
                    if (binOp) {
                        Expr* lhs = binOp->getLHS();
                        string ostreamString;
                        raw_string_ostream rawStringOStream(ostreamString);
                        PrintingPolicy Policy = this->context->getPrintingPolicy();
                        lhs->printPretty(rawStringOStream, nullptr, Policy);
                        string varName = rawStringOStream.str();

                        if (parentFunction)
                            parentFunction->getFunctionInfo()->allocaVars.push_back(varName);

                        /*Add the var to the table, marking it as needed to be freed.*/
                        VarTableTuple_t newTuple(
                            "", "", "", false, false, parentNode->getID(), "", "", "", true);
                        this->myDFG.insertVarTupleToTable(varName, newTuple);

                        /*Add a new stmt to store the recently created memory segment and delete it
                         * later*/
                        this->insertNullNodeAfter(parentStmtNode, NULLSTMT_ALLOCA, varName);
                    }
                } else if (check_isa<DeclStmt>(parentStmtNode->getStmt())) {
                    DeclStmt* declStmt = dyn_cast<DeclStmt>(parentStmtNode->getStmt());
                    if (declStmt) {
                        for (Decl** declIt = declStmt->decl_begin(); declIt != declStmt->decl_end();
                             declIt++) {
                            string varName = getNameFromDecl(*declIt);

                            if (parentFunction)
                                parentFunction->getFunctionInfo()->allocaVars.push_back(varName);

                            /*Add the var to the table, marking it as needed to be freed.*/
                            VarTableTuple_t newTuple(
                                "", "", "", false, false, parentNode->getID(), "", "", "", true);
                            this->myDFG.insertVarTupleToTable(varName, newTuple);

                            /*Add a new stmt to store the recently created memory segment and delete
                             * it later*/
                            this->insertNullNodeAfter(parentStmtNode, NULLSTMT_ALLOCA, varName);
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool ompASTVisitor::VisitOMPAtomicDirective(OMPAtomicDirective* s)
{
    AtomicNode* newAtomicNode
        = new AtomicNode(s, this->currentNode->parent, this->currentNode->getID());

    vector<OMPClause*> ompClauses = s->clauses().vec();
    for (OMPClause* clause : ompClauses) {
        if (dyn_cast<OMPReadClause>(clause)) {
            newAtomicNode->ompClause = "read";
        } else if (dyn_cast<OMPWriteClause>(clause)) {
            newAtomicNode->ompClause = "write";
        }
    }

    DFGNode* oldCurrentNode = this->currentNode;
    this->currentNode->parent->myNodes.replace(this->currentNode, newAtomicNode);
    this->currentNode = newAtomicNode;
    delete (oldCurrentNode);

    return true;
}

bool ompASTVisitor::VisitOMPExecutableDirective(OMPExecutableDirective* s)
{
    /*Clear the task scope we are keeping for the depend clause.*/
    if (check_isa<OMPParallelDirective>(s))
        this->clearDependTaskNodes();

    if (check_isa<OMPForDirective>(s) || check_isa<OMPParallelForDirective>(s)) {
        this->currentNode->myLoopInfo = new LoopInfo(this->context);
        this->currentNode->myLoopInfo->context = this->context;
    }

    else if (check_isa<OMPCriticalDirective>(s)) {
        OMPCriticalDirective* criticalDirective = dyn_cast<OMPCriticalDirective>(s);
        this->currentNode->criticalDirectiveName
            = criticalDirective->getDirectiveName().getAsString();
    }

    else if (check_isa<OMPTaskDirective>(s)) {
        this->currentNode->myTaskInfo = new TaskInfo(this->currentNode);
    }

    /* Examine the omp region's clauses */
    vector<OMPClause*> ompClauses = s->clauses().vec();
    for (OMPClause* clause : ompClauses) {
        /* private(..) */
        PrintingPolicy Policy = context->getPrintingPolicy();
        if (dyn_cast<OMPPrivateClause>(clause)) {
            OMPPrivateClause* C = dyn_cast<OMPPrivateClause>(clause);
            for (OMPPrivateClause::varlist_iterator I = C->varlist_begin(); I != C->varlist_end();
                 ++I) {
                DFGNode* ompNode = this->currentNode;
                if (!(ompNode->getStmt() && isa<OMPExecutableDirective>(ompNode->getStmt())))
                    ompNode = ompNode->findOMPParent();

                if (ompNode) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();

                    /* Look for the var's type in the var table */
                    string varType = "";
                    string varArray = "";
                    VarTableTuple_t varTuple;
                    bool found
                        = this->myDFG.getVarTupleFromTable(this->currentNode, varName, varTuple);
                    if (found) {
                        varType = get<0>(varTuple);
                        varArray = get<6>(varTuple);
                    }

                    OMP_VARINFO newTuple(varType, OMP_PRIVATE, -1, "", varArray, false);
                    pair<string, OMP_VARINFO> newInput(varName, newTuple);
                    ompNode->ompInputs.insert(newInput);
                }
            }
        }
        /* firstprivate(..) */
        else if (dyn_cast<OMPFirstprivateClause>(clause)) {
            OMPFirstprivateClause* C = dyn_cast<OMPFirstprivateClause>(clause);
            for (OMPFirstprivateClause::varlist_iterator I = C->varlist_begin();
                 I != C->varlist_end(); ++I) {
                DFGNode* ompNode = this->currentNode;
                if (!(ompNode->getStmt() && isa<OMPExecutableDirective>(ompNode->getStmt())))
                    ompNode = ompNode->findOMPParent();

                if (ompNode) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();

                    /* Look for the var's type in the var table */
                    string varType = "";
                    string varArray = "";
                    VarTableTuple_t varTuple;
                    bool found
                        = this->myDFG.getVarTupleFromTable(this->currentNode, varName, varTuple);
                    if (found) {
                        varType = get<0>(varTuple);
                        varArray = get<6>(varTuple);
                    }
                    OMP_VARINFO newTuple(varType, OMP_FIRSTPRIVATE, -1, "", varArray, false);
                    pair<string, OMP_VARINFO> newInput(varName, newTuple);
                    ompNode->ompInputs.insert(newInput);
                }
            }
        }
        /* lastprivate(..) */
        else if (dyn_cast<OMPLastprivateClause>(clause)) {
            OMPLastprivateClause* C = dyn_cast<OMPLastprivateClause>(clause);
            for (OMPLastprivateClause::varlist_iterator I = C->varlist_begin();
                 I != C->varlist_end(); ++I) {
                DFGNode* ompNode = this->currentNode;
                if (!(ompNode->getStmt() && isa<OMPExecutableDirective>(ompNode->getStmt())))
                    ompNode = ompNode->findOMPParent();

                if (ompNode) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();

                    /* Look for the var's type in the var table */
                    string varType = "";
                    string varArray = "";
                    VarTableTuple_t varTuple;
                    bool found
                        = this->myDFG.getVarTupleFromTable(this->currentNode, varName, varTuple);
                    if (found) {
                        varType = get<0>(varTuple);
                        varArray = get<6>(varTuple);
                    }

                    OMP_VARINFO newTuple(varType, OMP_LASTPRIVATE, -1, "", varArray, false);
                    pair<string, OMP_VARINFO> newInput(varName, newTuple);
                    ompNode->ompInputs.insert(newInput);
                }
            }
        }
#if 0
/*TODO: clang does not identify this clause for structured-blocks*/
		/*ordered */
		else if (dyn_cast < OMPOrderedClause > (clause)) {
			outs() << "Ordered clause node " << this->currentNode->getID()
				<< " this->currentNode->isOMPParallel " << this->
				currentNode->isOMPParallel()
				<< " this->currentNode->isOMPFor " << this->
				currentNode->isOMPFor()
				<< "\n";
		}
#endif
        /* nowait */
        else if (dyn_cast<OMPNowaitClause>(clause)) {
            this->currentNode->noWait = true;
        }
        /* codelet clause */
        else if (dyn_cast<OMPCodeletClause>(clause)) {
            this->currentNode->ompExtensionClause->codelet = true;
        }
        /* schedule(..) */
        else if (dyn_cast<OMPScheduleClause>(clause)) {
            OMPScheduleClause* sch = dyn_cast<OMPScheduleClause>(clause);

            OMP_SCHEDULE schedPolicy = (OMP_SCHEDULE)sch->getScheduleKind();
            this->currentNode->myLoopInfo->schedulingPolicy.first = schedPolicy;

            Expr* chunkSizeExpr = sch->getChunkSize();
            if (chunkSizeExpr) {
                PrintingPolicy Policy = context->getPrintingPolicy();
                string ostreamString;
                raw_string_ostream rawStringOStream(ostreamString);
                chunkSizeExpr->printPretty(rawStringOStream, nullptr, Policy);
                get<0>(this->currentNode->myLoopInfo->schedulingPolicy.second) = chunkSizeExpr;

            } else {
                get<0>(this->currentNode->myLoopInfo->schedulingPolicy.second) = nullptr;
                get<1>(this->currentNode->myLoopInfo->schedulingPolicy.second) = 1;
            }
        }
        /* reduction(..) */
        else if (dyn_cast<OMPReductionClause>(clause)) {
            OMPReductionClause* red = dyn_cast<OMPReductionClause>(clause);

            /*The reduction operator is a number following the list in the OpenMP 3.1 standard, pp
             * 103:
             * + : 1 : initialization value : 0
             * * : 2 : initialization value : 1
             * - : 3 : initialization value : 0
             * & : 4 : initialization value : ~0
             * | : 5 : initialization value : 0
             * ^ : 6 : initialization value : 0
             * && : 7 : initialization value : 1
             * || : 8 : initialization value : 0
             * max : 9 : initialization value : least representable value in the variable's type
             * min : 10 : initialization value : largest representable value in the variable's type
             * */
            string redOperator = "";
            llvm::iterator_range<MutableArrayRef<Expr*>::iterator> reduction_ops
                = red->reduction_ops();
            for (MutableArrayRef<Expr*>::iterator it = reduction_ops.begin();
                 it < reduction_ops.end(); it++) {
                if (isa<BinaryOperator>(*it)) {
                    BinaryOperator* bin_it = dyn_cast<BinaryOperator>(*it);
                    Expr* rhs = bin_it->getRHS();
                    if (isa<BinaryOperator>(rhs)) {
                        BinaryOperator* bin_it = dyn_cast<BinaryOperator>(rhs);
                        StringRef op = bin_it->getOpcodeStr();
                        redOperator = op;
                    } else {
                        Expr* lhs = bin_it->getRHS();
                        if (isa<CastExpr>(lhs)) {
                            /*This only happens for min and max. CastExpr documentation does not say
                               how
                               to get the operator so let's do it manually. */
                            CastExpr* cast_it = dyn_cast<CastExpr>(lhs);
                            Expr* subExpr = cast_it->getSubExprAsWritten();
                            string ostreamString;
                            raw_string_ostream rawStringOStream(ostreamString);
                            PrintingPolicy Policy = context->getPrintingPolicy();
                            subExpr->printPretty(rawStringOStream, nullptr, Policy);
                            if (rawStringOStream.str().find("<") != std::string::npos)
                                redOperator = "min";
                            else if (rawStringOStream.str().find(">") != std::string::npos)
                                redOperator = "max";
                        }
                    }
                }
            }
            for (OMPReductionClause::varlist_iterator I = red->varlist_begin();
                 I != red->varlist_end(); ++I) {
                DFGNode* ompNode = this->currentNode;

                if (check_isa<OMPExecutableDirective>(ompNode->getStmt()) == false)
                    ompNode = ompNode->findOMPParent();

                if (ompNode && ompNode->getLoopInfo()) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();

                    ReductionVarTuple_t newTuple(redOperator, "", "", "");
                    ReductionVarPair_t newPair(varName, newTuple);

                    ReductionVar_t::iterator it
                        = ompNode->getLoopInfo()->reductionVars.find(varName);
                    if (it == ompNode->getLoopInfo()->reductionVars.end())
                        ompNode->getLoopInfo()->reductionVars.insert(newPair);
                } else if (check_isa<OMPParallelDirective>(ompNode->getStmt())) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();

                    ReductionVarTuple_t newTuple(redOperator, "", "", "");
                    ReductionVarPair_t newPair(varName, newTuple);

                    ReductionVar_t::iterator it = ompNode->parallelReductionVars.find(varName);
                    if (it == ompNode->parallelReductionVars.end())
                        ompNode->parallelReductionVars.insert(newPair);
                }
            }
        }
        /* copyin(..) */
        else if (dyn_cast<OMPCopyinClause>(clause)) {
            OMPCopyinClause* cop = dyn_cast<OMPCopyinClause>(clause);

            DFGNode* ompNode = this->currentNode;
            if (check_isa<OMPExecutableDirective>(ompNode->getStmt()) == false) {
                ompNode = ompNode->findOMPParent();
            }
            if (ompNode) {
                ompNode->myCopyinClause = new CopyinClause(ompNode);

                for (OMPCopyinClause::varlist_iterator I = cop->varlist_begin();
                     I != cop->varlist_end(); ++I) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();

                    CopyinVarPair_t newPair(varName, "");
                    ompNode->getCopyinClause()->vars.insert(newPair);
                }
            }
        }
        /* copyprivate(..) */
        else if (dyn_cast<OMPCopyprivateClause>(clause)) {
            OMPCopyprivateClause* cop = dyn_cast<OMPCopyprivateClause>(clause);

            DFGNode* ompNode = this->currentNode;
            if (check_isa<OMPExecutableDirective>(ompNode->getStmt()) == false) {
                ompNode = ompNode->findOMPParent();
            }
            if (ompNode) {
                ompNode->myCopyPrivateClause = new CopyPrivateClause(ompNode);

                for (OMPCopyprivateClause::varlist_iterator I = cop->varlist_begin();
                     I != cop->varlist_end(); ++I) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();

                    CopyinVarPair_t newPair(varName, "");
                    ompNode->getCopyPrivateClause()->vars.insert(newPair);
                }
            }
        }
        /* depend() */
        else if (dyn_cast<OMPDependClause>(clause)) {
            /*
            dependClause->getDependencyKind () gives the type of dependency:
            in = 0
            out = 1
            inout = 2
            */

            DFGNode* ompNode = this->currentNode;
            if (ompNode && ompNode->getTaskInfo()) {
                OMPDependClause* dependClause = dyn_cast<OMPDependClause>(clause);

                int depType = (unsigned int)(dependClause->getDependencyKind());

                for (OMPDependClause::varlist_iterator I = dependClause->varlist_begin();
                     I != dependClause->varlist_end(); ++I) {
                    string ostreamString;
                    raw_string_ostream rawStringOStream(ostreamString);
                    (*I)->printPretty(rawStringOStream, nullptr, Policy);

                    string varName = rawStringOStream.str();
                    ompNode->getTaskInfo()->insertDependClauseVar(varName, depType);
                    DFGNode* previousTaskNode
                        = this->findDependencyInPreviousTask(varName, depType);

                    if (previousTaskNode) {
                        ompNode->getTaskInfo()->insertInputDependencyNode(previousTaskNode);
                        previousTaskNode->getTaskInfo()->insertOutputDependencyNode(ompNode);
                    }
                }

                this->insertDependTaskNode(ompNode);
            }
        }
    }

    if (check_isa<OMPSectionDirective>(s) || check_isa<OMPCriticalDirective>(s)) {
        this->currentNode->noWait = true;
    } else if (check_isa<OMPTaskwaitDirective>(s)) {
        this->currentNode->noWait = false;
    } else if (check_isa<OMPMasterDirective>(s)) {
        this->currentNode->noWait = true;
    }
    return true;
}

DFGNode* ompASTVisitor::findDependencyInPreviousTask(std::string varName, unsigned int depType)
{

    DFGNode* taskNode = nullptr;

    /*Current dependency is in, therefore we need to look for previous tasks having
    same variable but with out or inout.*/
    if (depType == 0) {
        for (std::vector<DFGNode*>::reverse_iterator it = this->dependTaskNodes.rbegin();
             it < this->dependTaskNodes.rend(); it++) {

            taskNode = *it;
            bool found = taskNode->getTaskInfo()->findDependClauseVar(varName, 1);
            if (found == false)
                found = taskNode->getTaskInfo()->findDependClauseVar(varName, 2);
            if (found)
                break;
        }
    }
    /*Current dependency is out, therefore we need to look for previous tasks having
    same variable but with in or inout.*/
    else if (depType == 1) {
        for (std::vector<DFGNode*>::reverse_iterator it = this->dependTaskNodes.rbegin();
             it < this->dependTaskNodes.rend(); it++) {

            taskNode = *it;
            bool found = taskNode->getTaskInfo()->findDependClauseVar(varName, 0);
            if (found == false)
                found = taskNode->getTaskInfo()->findDependClauseVar(varName, 2);
            if (found)
                break;
        }
    }
    /*Current dependency is inout, therefore we need to look for previous tasks having
    same variable but with in or out.*/
    else if (depType == 2) {
        for (std::vector<DFGNode*>::reverse_iterator it = this->dependTaskNodes.rbegin();
             it < this->dependTaskNodes.rend(); it++) {

            taskNode = *it;
            bool found = taskNode->getTaskInfo()->findDependClauseVar(varName, 0);
            if (found == false)
                found = taskNode->getTaskInfo()->findDependClauseVar(varName, 1);
            if (found)
                break;
        }
    }

    return taskNode;
}

bool ompASTVisitor::VisitOMPTaskDirective(clang::OMPTaskDirective* s)
{
    if (this->currentNode->myTaskInfo == nullptr)
        this->currentNode->myTaskInfo = new TaskInfo(this->currentNode);

    this->currentNode->noWait = true;

    return true;
}

bool ompASTVisitor::VisitCapturedStmt(CapturedStmt* s)
{
    for (auto capture = s->capture_begin(); capture != s->capture_end(); capture++) {
        VarDecl* varDecl = capture->getCapturedVar();
        Decl* decl = dyn_cast<Decl>(varDecl);

        DFGNode* ompNode = this->currentNode;

        if (decl != nullptr) {
            string varName = getNameFromDecl(decl);

            /* DFGNode* ompNode = this->currentNode;  */
            if ((check_isa<OMPExecutableDirective>(ompNode->getStmt())) == false) {
                ompNode = ompNode->findOMPParent();
            }
            /* if the omp node parent is not a task or a parallel region, then
               the data-sharing attributes of the variable are inherited
               from the enclosing context.
               string varName = getNameFromDecl(decl); */
            OMP_VARSCOPE varScope = OMP_SHARED;
            if (check_isa<OMPParallelForDirective>(ompNode->getStmt()) == false
                && check_isa<OMPParallelDirective>(ompNode->getStmt()) == false
                && check_isa<OMPTaskDirective>(ompNode->getStmt()) == false) {
                DFGNode* enclosingParentNode = ompNode->findParentNode();
                if (enclosingParentNode) {
                    TPInputs_t::iterator it = enclosingParentNode->ompInputs.find(varName);
                    if (it != enclosingParentNode->ompInputs.end()) {
                        varScope = get<1>(it->second);
                    } else {
                        it = enclosingParentNode->ompVariables.find(varName);
                        if (it != enclosingParentNode->ompVariables.end()) {
                            varScope = get<1>(it->second);
                        }
                    }

                    if (varScope == OMP_PRIVATE) {
                        /* If the variable is private in the enclosing statement but it's not
                         * declared
                         * as private in the current region, then any value written in the current
                         * region
                         * must be seen in the enclosing after the current region ends, so we need
                         * every
                         * codelet to have a pointer to the variable on the enclosing region, which
                         * we
                         * call OMP_SHARED_PRIVATE.
                         * */
                        varScope = OMP_SHARED_PRIVATE;
                    }
                }
            }

            if (ompNode) {
                ompNode->addNewInput(decl, varScope);
            }
        }
    }
    return true;
}

void ompASTVisitor::addNewVarToTable(clang::VarDecl* d)
{
    /*Get the name */
    string varName = getNameFromDecl(d);

    /* Add var to varTable */

    /*Get the type */
    string varType = getTypeFromDecl(d);

    size_t pos = varType.find("const");
    if (pos != string::npos)
        varType.erase(pos, 5);

    string varTypeTemp = varType;

    string varArray = "";
    string varSize = "";
    size_t pos0 = varTypeTemp.find("[");
    size_t pos1 = varTypeTemp.find_last_of("]");
    if (pos0 != string::npos && pos1 != string::npos) {
        varType = varTypeTemp.substr(0, pos0);
        varArray = varTypeTemp.substr(pos0, pos1 - 1);
        varSize = varTypeTemp.substr(pos0 + 1, pos1 - 1 - (pos0 + 1) + 1);
    }

    /*Get the storage specifier */
    string storageSpec = "";
    if (d->hasExternalStorage())
        storageSpec = "extern";
    else if (d->isStaticLocal())
        storageSpec = "static";

    bool isGlobal = true;
    int parentNodeID = 0;

    string initValStr = "";
    if (d->hasInit()) {
        Expr* initExpr = d->getInit();
        if (initExpr) {
            PrintingPolicy Policy = this->context->getPrintingPolicy();
            string ostreamString;
            raw_string_ostream rawStringOStream(ostreamString);
            initExpr->printPretty(rawStringOStream, nullptr, Policy);
            initValStr = rawStringOStream.str();
        }
    }

    VarTableTuple_t newTuple(varType, initValStr, storageSpec, isGlobal, false, parentNodeID,
        varArray, "", varSize, false);

    this->myDFG.insertVarTupleToTable(varName, newTuple);
}

bool ompASTVisitor::VisitVarDecl(VarDecl* d)
{
    DFGNode* parentNode = this->currentNode->findParentNode();

    /*Get the name */
    string varName = getNameFromDecl(d);

    /* Add var to its parentNode if found */
    if (parentNode)
        parentNode->addNewVar(d);

    /* Add var to varTable */

    /*Get the type */
    string varType = getTypeFromDecl(d);

    size_t pos = varType.find("const");
    if (pos != string::npos)
        varType.erase(pos, 5);

    string varTypeTemp = varType;

    string varArray = "";
    string varSize = "";
    size_t pos0 = varTypeTemp.find("[");
    size_t pos1 = varTypeTemp.find_last_of("]");
    if (pos0 != string::npos && pos1 != string::npos) {
        varType = varTypeTemp.substr(0, pos0);
        varArray = varTypeTemp.substr(pos0, pos1 - 1);
        varSize = varTypeTemp.substr(pos0 + 1, pos1 - 1 - (pos0 + 1) + 1);
    }

    /*Get the storage specifier */
    string storageSpec = "";
    if (d->hasExternalStorage())
        storageSpec = "extern";
    else if (d->isStaticLocal())
        storageSpec = "static";

    /* If the parentnode for this declaration is root node, then it's a
     * global var. If not, get the ID of the parentNode where declared.*/
    bool isGlobal = false;
    int parentNodeID = -1;
    if (parentNode->getName().compare("rootNode") == 0) {
        isGlobal = true;
        parentNodeID = 0;
    } else if (parentNode) {
        parentNodeID = parentNode->getID();
    }

    string initValStr = "";
    if (d->hasInit()) {
        Expr* initExpr = d->getInit();
        if (initExpr) {
            PrintingPolicy Policy = this->context->getPrintingPolicy();
            string ostreamString;
            raw_string_ostream rawStringOStream(ostreamString);
            initExpr->printPretty(rawStringOStream, nullptr, Policy);
            initValStr = rawStringOStream.str();
        }
    }
    VarTableTuple_t newTuple(varType, initValStr, storageSpec, isGlobal, false, parentNodeID,
        varArray, "", varSize, false);
    this->myDFG.insertVarTupleToTable(varName, newTuple);
    return true;
}

bool ompASTVisitor::VisitOMPThreadPrivateDecl(clang::OMPThreadPrivateDecl* d)
{
    /* Check for variables being declared as threadprivate. This pragma
     * appears outside a parallel region and it's usually for
     * global vars. Since this pragma has no associated omp region,
     * we use the vartable*/
    ArrayRef<const Expr*>::iterator it = d->varlist_begin();
    while (it != d->varlist_end()) {
        /* Get the name */
        PrintingPolicy Policy = this->context->getPrintingPolicy();
        Policy.darts_printingInTP = false;

        string ostreamString;
        raw_string_ostream rawStringOStream(ostreamString);
        (*it)->printPretty(rawStringOStream, nullptr, Policy);
        string varName = rawStringOStream.str();

        /* Find the var in the table */
        VarTable2_t::iterator varIt2 = this->myDFG.varTable2.find(varName);
        if (varIt2 != this->myDFG.varTable2.end())
            for (size_t i = 0; i < get<5>(varIt2->second).size(); i++) {
                get<4>(varIt2->second)[i] = true;
            }
        it++;
    }

    return true;
}

bool ompASTVisitor::VisitParmVarDecl(clang::ParmVarDecl* d)
{
    DFGNode* functionNode = this->currentNode;
    if (functionNode->myFunctionInfo == nullptr) {
        functionNode = functionNode->findFunctionParent();
    }

    if (functionNode) {
        int varPos = -1;
        if (functionNode->myFunctionInfo) {
            varPos = functionNode->myFunctionInfo->numOfArgs;
            functionNode->myFunctionInfo->numOfArgs = functionNode->myFunctionInfo->numOfArgs + 1;
        }

        string varName = getNameFromDecl(d);
        TPInputs_t::iterator it = functionNode->ompVariables.find(varName);

        if (it != functionNode->ompVariables.end()) {
            get<2>(it->second) = varPos;
        }
    }
    return true;
}

bool ompASTVisitor::VisitTagDecl(clang::TagDecl* d) { return true; }

bool ompASTVisitor::VisitEnumConstantDecl(clang::EnumConstantDecl* d)
{
    if (this->myDFG.numEnumTypesPrinted > 0) {
        constantsDeclStream << ", ";
    }
    constantsDeclStream << d->getNameAsString();
    this->myDFG.numEnumTypesPrinted++;

    return true;
}
}
