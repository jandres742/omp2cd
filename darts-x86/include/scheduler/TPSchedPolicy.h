#pragma once

/**
 * Title: TP Scheduler policies
 * 
 * This file describes the TP policy functions that are currently implemented. The TP policy
 * can change to fit to your particular program in order to improve performance. You can also
 * implement your own in this file
 * 
 * See Also:
 * <TPScheduler>
 * <ThreadedProcedure>
 */

#include <list>
#include <queue>

#include "threadlocal.h"

namespace darts {

class TPScheduler;

typedef bool (*tp_policy)(TPScheduler*);

/*General TP policy:
     * The TP scheduler is responsible for load balancing TP closures
     * across between clusters.  In doing this a typical approach will
     * be the following:
     * 1. Try to see if there are any local TP closures
     * 2. If there are no local TP closures try to steal
     * 3. If there is a TP closure (local or stolen) unpack it
     * 4. Distribute the unpacked codelets to Micro schedulers
     * 
     * Most changes to the policies will alter step 4. (order of distribution)
     * or add step 5. which is if the Micro schedulers are "full"
     * then work on a codelet myself
     */

/*This TP policy differs by pushing one codelet to a micro scheduler, and then
     tries to push the next.  If a scheduler is full, it cycles to the next.  It 
     will try each micro scheduler once.  If they are all full, it will perform the
     codelet itself, and upon completion, will resume scheduling codelets to the micro
     schedulers.*/
bool TPPushFull(TPScheduler* myTPS);

/*This TP policy cycles through its micro schedulers looking for a one to run a
     codelet.  If the micro scheduler is full, the TP scheduler moves on to the next
     one.  The TP scheduler never runs any codelets*/
bool TPRoundRobin(TPScheduler* myTPS);

/*While the TPPushFull policy can run with no micro schedulers, this single level
     policy does not have the overhead in checking if it has micro schedulers. This
     policy is intended to run with the whole system acting as TP schedulers.  A
     TP scheduler will follow general steps 1 and 2, but instead of distributing
     the codelets, it will run them.*/
bool TPSingleLevel(TPScheduler* myTPS);

/*This TP policy is intended to run with MicroSteal Policy!  In TPTwoLevelSteal
     TP closure are balanced and unpacked per general step 1,2,3 but the codelets
     are not distributed.  Instead the TP scheduler begins working on codelets.
     during this time the micro schedulers will steal codelets from the TP scheduler.
     Once there are no more codelets for the TP scheduler to run, the TP scheduler
     goes back to general steps 1 and 2.*/
bool TPTwoLevelSteal(TPScheduler* myTPS);

/*Like TPTwoLevelSteal but includes a downtime policy and TP deletion deferral before sleeping*/
bool TPTwoLevelSteal_Sleep(TPScheduler* myTPS);

/*Like TPTwoLevelSteal but includes a downtime policy and TP deletion deferral is done only at the end*/
bool TPTwoLevelSteal_DeleteAtFinal(TPScheduler* myTPS);
}
