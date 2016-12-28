#pragma once

#include <queue>

namespace darts {
class MScheduler;

typedef bool (*mc_policy)(MScheduler*);

/*General Micro policy:
     * The Micro scheduler is responsible for running the codelets localy
     * on a single core. The general steps are as follows:
     * 1. Check for a local codelet
     * 2. If there is a codelet run it
     * 3. Do so clean up if the TP is complete
     * 4. Repeat
     * 
     * Changes can occure on where to find codelets
     */

/*This is the standard policy which follows the general steps*/
bool MicroStandard(MScheduler* mSched);
/*This policy is intended for use with TPTwoLevelSteal!
     In this policy the micro scheduler pops a codelet from its
     parent TP scheduler instead of having the scheduler distribute
     codelets.*/
bool MicroSteal(MScheduler* mSched);

/*Like MicroSteal but includes a downtime policy and TP deletion deferral before sleeping*/
bool MicroSteal_Sleep(MScheduler* mSched);

/*Like MicroSteal but includes a downtime policy and TP deletion deferral is done only at the end*/
bool MicroSteal_DeleteAtFinal(MScheduler* mSched);
}
