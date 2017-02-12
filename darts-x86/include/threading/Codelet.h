#pragma once
#include "ABCScheduler.h"
#include "SyncSlot.h"
#include "codeletDefines.h"
#include <stdint.h>

#include <mutex>

namespace darts {

// This is a forward declaration since there is a circular dependence
class ThreadedProcedure;
class ABCScheduler;

/*
                 * Class: Codelet
                 * The codelet class is a virutal class. Use this class to instantiate codelets
                 * and provide a funct method to execute.
                 *
                 * See Also:
                 * <ThreadedProcedure>
                 * <ABCScheduler>
    */

class Codelet {
private:
    /*
                                 * Variable: sync_
                                 * The codelets counter
                                */
    SyncSlot sync_;

    /*
                                 * Variable: status_
                                 * The status of the codelet TODO: Explicit?
                                */
    uint32_t status_;

protected:
    /*
                                 * Variable: myTP_
                                 * Pointer to TP frame/context
                                */
    ThreadedProcedure* myTP_;

private:
    /*myGlobalID is the ID given to the codelet globally. For instance, if N threads are being
 * used in the application, myGlobalID may have any value from 0 to N -1.
 * myLocalID is the ID given to a codelet locally when parallel loop is decomposed as several
 * similar TPs. For instance, if N threads are being used for the parallel loop, and such loop
 * is decomposed in S similar TPs, then myLocalID may have any vlue from 0 to N/M - 1
 * myLocalID is mainly used to access arrays that are local to any of the S TPs (myGlobalID
 * cannot be used on that case since the local array may be only N/M positions long. */
    uint32_t myLocalID;
    uint32_t myGlobalID;

public:
    /**
                                 * Constructor: Codelet(uint32_t dep, uint32_t res,
      *ThreadedProcedure * theTp, uint32_t stat);
                                 *
                                 * Parameters:
                                 *	dep - The dependence counter of the codelet
                                 *	res - The value of the dependence counter if the codelet has
      *to be reset
                                 *	theTp - The TP the codelet belongs to
                                 *	stat - Locality parameter (TODO: has to be explicited)
         */
    Codelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp = NULL, uint32_t stat = SHORTWAIT);
    Codelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp, uint32_t stat, uint32_t _localID);
    Codelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp, uint32_t stat, uint32_t _localID,
        uint32_t _globalID);
    Codelet(void);
    Codelet(Codelet& original_codelet);
    Codelet& operator=(const Codelet& rhs_codelet);

    // Destructors
    virtual ~Codelet() {}

    /**
                                 * Method: initCodelet
         * Implements a delayed Codelet init. We may need them for an array of codelets
                                 * when the constructor is not called
                                 *
                                 * Parameters:
                                 *	dep - The dependence counter of the codelet
                                 *	res - The value of the dependence counter if the codelet has
      *to be reset
                                 *	theTp - The TP the codelet belongs to
                                 *	stat - Locality parameter (TODO: has to be explicited)
         */
    void initCodelet(uint32_t dep, uint32_t res, ThreadedProcedure* theTp, uint32_t stat);

    /**
                                 * Method: decDep
         * Decrements the dependence counter of the codelet
         */
    virtual void decDep(void);

    inline void incDep(void) { sync_.incOnlyCounter(); }

    /**
                                 * Method: setDep
         * Set the dependence counter of the codelet
         */
    virtual void setDep(uint32_t dep);

    /**
                                 * Method: resetCodelet
         * Resets the codelet
         */
    void resetCodelet(void);

    /**
                                 * Method: resetCodelet
         * Returns:
                                 * Whether or not the codelet counter is 0. If it is, all the
     * dependences have been satisfied and
                                 * the codelet can now be fired
         */
    inline bool codeletReady(void) { return sync_.ready(); }

    /**
                                 * Method: setStatus
                                 * Sets the status of the codelet (TODO: Has to be explained)
         */
    inline void setStatus(uint32_t stat) { status_ = stat; }

    /**
                                 * Method: getStatus
                                 * Gets the status of the codelet (TODO: Has to be explained)
         */
    inline uint32_t getStatus(void) const { return status_; }

    /**
                                 * Method: getTP
                                 * Returns:
                                 * The parent ThreadedProcedure pointer
         */
    //~ ThreadedProcedure * getTP(void);
    inline ThreadedProcedure* getTP(void) { return myTP_; }

    /**
                                 * Method: setTP
                                 * Sets the parent ThreadedProcedure pointer
         */
    void setTP(ThreadedProcedure* aTP);

    /**
                                 * Method: casStatus
                                 * TODO description
         */
    bool casStatus(uint32_t oldval, uint32_t newval);

    /**
                                 * Method: Do
                                 * Wrapper to push codelets to the parent scheduler
         */
    void add(Codelet* aCodelet);

    /**
                                 * Method: getCounter
                                 * Returns:
                                 * The dependence counter of the codelet
         */
    uint32_t getCounter(void) const;

    SyncSlot* getSyncSlot(void);

    /**
                                 * Method: funct
                                 * This is the code of the codelet to be executed.
         */
    virtual void fire(void) = 0;

    inline uint32_t getLocalID() { return this->myLocalID; }

    inline uint32_t getGlobalID() { return this->myGlobalID; }

    inline uint32_t getID() { return this->myGlobalID; }

    inline void setLocalID(uint32_t codeletID) { this->myLocalID = codeletID; }

    inline void setGlobalID(uint32_t codeletID) { this->myGlobalID = codeletID; }

    inline void setID(uint32_t codeletID) { this->myGlobalID = codeletID; }

    inline uint32_t omp_get_thread_num() { return this->myGlobalID; }

#ifdef TRACE
    void* returnFunct(void);
#endif
};

} // namespace darts
