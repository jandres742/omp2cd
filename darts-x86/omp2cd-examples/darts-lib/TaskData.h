#ifndef _TASKDATA_H_
#define _TASKDATA_H_

#include "utils.h"
#include <stack>
#include <list>
#include <deque>
class TaskData
{
public:
	TaskData():
	taskCompleted(false),
	nextSyncCodelet(nullptr)
	{
	}

	TaskData(darts::Codelet* in_nextSyncCodelet):
	taskCompleted(false),
	nextSyncCodelet(in_nextSyncCodelet)
	{
	}

	/*Termination flag to be used by in the depend clauses*/
	bool taskCompleted;
	/*Synchronization codelet for this task*/
	darts::Codelet* nextSyncCodelet;
	
	std::deque<void*> taskInOutVarDependencies;	
	std::vector<TaskData**> taskDependencies;
	std::list<void*> taskInVarDependencies;
	std::list<void*> taskOutVarDependencies;

	void setDeps(TaskData* prevTaskInput)
	{
		if(prevTaskInput == nullptr)
			return;

		bool taskAdded = false;
		/*Dependencies wrt depend(in) vars*/
		for (void* depPtr : prevTaskInput->taskInVarDependencies) {
			for (void* targetDepPtr : this->taskOutVarDependencies) {
				if (depPtr == targetDepPtr) {
					TaskData* tempTaskData = (TaskData*)prevTaskInput;
					this->taskDependencies.push_back(&tempTaskData);
					taskAdded = true;
					break;
				}
			}
			if (taskAdded == false) {
				for (void* targetDepPtr : this->taskInOutVarDependencies) {
					if (depPtr == targetDepPtr) {
						TaskData* tempTaskData = (TaskData*)prevTaskInput;
						this->taskDependencies.push_back(&tempTaskData);
						taskAdded = true;
						break;
					}
				}
			}
		}
		/*Dependencies wrt depend(out) vars*/
		if (taskAdded == false) {
			for (void* depPtr : prevTaskInput->taskOutVarDependencies) {
				for (void* targetDepPtr : this->taskInVarDependencies) {
					if (depPtr == targetDepPtr) {
						TaskData* tempTaskData = (TaskData*)prevTaskInput;
						this->taskDependencies.push_back(&tempTaskData);
						taskAdded = true;
						break;
					}
				}
				if (taskAdded == false) {
					for (void* targetDepPtr : this->taskOutVarDependencies) {
						if (depPtr == targetDepPtr) {
							TaskData* tempTaskData = (TaskData*)prevTaskInput;
							this->taskDependencies.push_back(&tempTaskData);
							taskAdded = true;
							break;
						}
					}
				}
				if (taskAdded == false) {
					for (void* targetDepPtr : this->taskInOutVarDependencies) {
						if (depPtr == targetDepPtr) {
							TaskData* tempTaskData = (TaskData*)prevTaskInput;
							this->taskDependencies.push_back(&tempTaskData);
							taskAdded = true;
							break;
						}
					}
				}
			}
		}
		/*Dependencies wrt depend(inout) vars*/
		if (taskAdded == false) {
			for (void* depPtr : prevTaskInput->taskInOutVarDependencies) {
				for (void* targetDepPtr : this->taskInVarDependencies) {
					if (depPtr == targetDepPtr) {
						TaskData* tempTaskData = (TaskData*)prevTaskInput;
						this->taskDependencies.push_back(&tempTaskData);
						taskAdded = true;
						break;
					}
				}
				if (taskAdded == false) {
					for (void* targetDepPtr : this->taskOutVarDependencies) {
						if (depPtr == targetDepPtr) {
							TaskData* tempTaskData = (TaskData*)prevTaskInput;
							this->taskDependencies.push_back(&tempTaskData);
							taskAdded = true;
							break;
						}
					}
				}
				if (taskAdded == false) {
					for (void* targetDepPtr : this->taskInOutVarDependencies) {
						if (depPtr == targetDepPtr) {
							TaskData* tempTaskData = (TaskData*)prevTaskInput;
							this->taskDependencies.push_back(&tempTaskData);
							taskAdded = true;
							break;
						}
					}
				}
			}
		}
	}

	bool checkForDeps()
	{
		bool allCompleted = true;
		if(this->taskDependencies.size() > 0)
		for(TaskData** taskDep: this->taskDependencies){
			if(*taskDep != nullptr && (*taskDep)->taskCompleted == false){
				allCompleted = false;
				break;
			}
		}
		return allCompleted;
	}

};

#endif
