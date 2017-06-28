#include "TaskData.h"

void TaskData::setDeps(TaskData* prevTaskInput)
{
	bool taskAdded = false;
	/*Dependencies wrt depend(in) vars*/
	for (void* depPtr : prevTaskInput->taskInVarDependencies) {
		for (void* targetDepPtr : this->taskOutVarDependencies) {
			if (depPtr == targetDepPtr) {
				this->taskDependencies.push_back(prevTaskInput->taskCompleted);
				taskAdded = true;
				break;
			}
		}
		if (taskAdded == false) {
			for (void* targetDepPtr : this->taskInOutVarDependencies) {
				if (depPtr == targetDepPtr) {
					this->taskDependencies.push_back(prevTaskInput->taskCompleted);
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
					this->taskDependencies.push_back(prevTaskInput->taskCompleted);
					taskAdded = true;
					break;
				}
			}
			if (taskAdded == false) {
				for (void* targetDepPtr : this->taskOutVarDependencies) {
					if (depPtr == targetDepPtr) {
						this->taskDependencies.push_back(prevTaskInput->taskCompleted);
						taskAdded = true;
						break;
					}
				}
			}
			if (taskAdded == false) {
				for (void* targetDepPtr : task25InOutVarDependencies) {
					if (depPtr == targetDepPtr) {
						this->taskDependencies.push_back(prevTaskInput->taskCompleted);
						taskAdded = true;
						break;
					}
				}
			}
		}
	}
	/*Dependencies wrt depend(inout) vars*/
	if (task25added == false) {
		for (void* depPtr : prevTaskInput->taskInOutVarDependencies) {
			for (void* targetDepPtr : this->taskInVarDependencies) {
				if (depPtr == targetDepPtr) {
					this->taskDependencies.push_back(prevTaskInput->taskCompleted);
					taskAdded = true;
					break;
				}
			}
			if (taskAdded == false) {
				for (void* targetDepPtr : this->taskOutVarDependencies) {
					if (depPtr == targetDepPtr) {
						this->taskDependencies.push_back(prevTaskInput->taskCompleted);
						taskAdded = true;
						break;
					}
				}
			}
			if (taskAdded == false) {
				for (void* targetDepPtr : task25InOutVarDependencies) {
					if (depPtr == targetDepPtr) {
						this->taskDependencies.push_back(prevTaskInput->taskCompleted);
						taskAdded = true;
						break;
					}
				}
			}
		}
	}
}

bool TaskData::checkForDeps()
{
	bool allCompleted = true;
	for(bool* taskDep: this->taskDependencies){
		if(*taskDep == false){
			allCompleted = false;
			break;
		}
	}
	return allCompleted;
}
