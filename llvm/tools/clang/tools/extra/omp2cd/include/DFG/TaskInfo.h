#ifndef _TASKINFO_H_
#define _TASKINFO_H_

#include "include/DFG/DFG.h"

namespace omp2cd_space {

class DFG;
class DFGNode;

/*A class to store info about an omp task*/
class TaskInfo {
public:
    TaskInfo(DFGNode* _taskNode);

    DFGNode* taskNode;
    TPRegion* myTaskRegionStart;
    TPRegion* myTaskRegionNext;
    bool encapsulateTaskData;
    int numTotalTasks;

    void printEncapsulateTaskData(std::ostringstream& outputStream);
    void printTaskDataClass(std::ostringstream& outputStream);
    void printTaskCode(std::ostringstream& outputStream, size_t nodePos);
    void setSyncPoints();
    void insertDependClauseVar(std::string varName, unsigned int depType);
    bool findDependClauseVar(std::string varName, unsigned int depType);
    void insertInputDependencyNode(DFGNode* node);
    void insertOutputDependencyNode(DFGNode* node);

    std::vector<std::string> dependClauseInVars;
    std::vector<std::string> dependClauseOutVars;
    std::vector<std::string> dependClauseInOutVars;

    std::vector<DFGNode*> inputDependencyNodes;
    std::vector<DFGNode*> outputDependencyNodes;

    void printInputDependencyNodes(std::ostringstream& outputStream, std::string prefixStr,
        std::string postfixStr, std::string frontDelimiter = ", ", std::string backDelimiter = " ");

    void printOutputDependencyNodes(std::ostringstream& outputStream, std::string prefixStr,
        std::string postfixStr, std::string frontDelimiter = ", ", std::string backDelimiter = " ");

    DFGNode* mySyncPoint;
    TPRegion* mySyncPointRegion;
    TPRegion* nextRegionAfterTask;
    DFGNode* nextNodeAfterTask;
    bool containsFunctionCall;
};
}
#endif
