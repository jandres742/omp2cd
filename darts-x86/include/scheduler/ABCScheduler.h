#pragma once
#include <stdint.h>
#include <vector>
#ifdef TRACE
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#define REC 100
#endif

#define PENDINGTHRESHOLD 50

extern uint64_t TPSleepTime;
extern uint64_t MCSleepTime;

namespace darts {
class ThreadedProcedure;
class Codelet;
struct tpClosure;

#ifdef TRACE
struct record {
    uint64_t cycle;
    void* funct;
    record(void)
        : cycle(0)
        , funct(0)
    {
    }
    record(uint64_t theCycle, void* fn)
        : cycle(theCycle)
        , funct(fn)
    {
    }
};
#endif

/*ABCScheduler: (Abstract Base Class Scheduler)
    ABCScheduler contains the minimal
    shared member and methods of a scheduler
    class.  
    */

enum Alive_t { isDead = 0,
    isAlive };

class ABCScheduler {
private:
    ABCScheduler** runtimeTPSchedulers;

    /*Max units per cluster w/o HT*/
    uint64_t numOfMaxUnitsPerCluster;
    uint64_t numSched;
    uint64_t numTPSched;
    uint64_t numMCSched;

    //The scheduler's parent scheduler
    ABCScheduler* parent_;

    //A vector of children schedulers
    std::vector<ABCScheduler*> subscheduler_;

    //Scheduler ID
    uint64_t id;

    volatile Alive_t alive_;

    // uint64_t globalID;
    uint64_t sleepTime;

#ifdef TRACE
    std::vector<record> schedTrace_;
#endif

public:
    //Constructors
    ABCScheduler(ABCScheduler* parent, std::vector<ABCScheduler*> child)
        : parent_(parent)
        , subscheduler_(child)
        , id(0)
        , alive_(isAlive)
        , sleepTime(1)
    {
    }

    ABCScheduler(void)
        : parent_(0)
        , id(0)
        , alive_(isAlive)
        , sleepTime(1)
    {
    }

    //Destuctors
    ~ABCScheduler()
    {
#ifdef TRACE
        printRecord();
#endif
    }

    //Getters and Setters for the parent scheduler
    ABCScheduler*
    getParentScheduler(void) const
    {
        return parent_;
    }

    void setParentScheduler(ABCScheduler* aSched)
    {
        parent_ = aSched;
    }

    //Getters and Setters for the Sub schedulers
    ABCScheduler*
    getSubScheduler(std::size_t pos) const
    {
        if (subscheduler_.size() > pos)
            return subscheduler_.at(pos);
        return 0;
    }

    void
    setSubScheduler(ABCScheduler* aSub)
    {
        subscheduler_.push_back(aSub);
    }

    //Returns the number of Sub Schedulers
    std::size_t getNumSub(void) const
    {
        return subscheduler_.size();
    }

    //Removes a specific Sub Scheduler
    void
    removeScheduler(ABCScheduler* aSub)
    {
        for (std::vector<ABCScheduler*>::iterator it = subscheduler_.begin(); it != subscheduler_.end(); ++it) {
            if ((*it) == aSub) {
                subscheduler_.erase(it);
                return;
            }
        }
    }

    inline void setID(uint64_t theID)
    {
        this->id = theID;
    }

    inline uint64_t getID(void)
    {
        return this->id;
    }

    inline void setSleepTime(uint64_t theSleep)
    {
        this->sleepTime = theSleep;
    }

    inline uint64_t getSleepTime(void)
    {
        return this->sleepTime;
    }

    inline void kill(void)
    {
        alive_ = isDead;
    }

    inline void resurrect(void)
    {
        alive_ = isAlive;
    }

    inline bool alive(void)
    {
        return (alive_ == isAlive);
    }

#ifdef TRACE

    void
    addRecord(uint64_t theCycle, void* fn)
    {
        schedTrace_.push_back(record(theCycle, fn));
    }

    void
    printRecord(void)
    {
        std::ofstream myfile;
        std::stringstream ss;
        ss << "thread" << id << ".txt";
        std::string s = ss.str();
        myfile.open(s.c_str());
        if (!myfile.is_open()) {
            std::cout << "Could not open " << s.c_str() << std::endl;
        } else {
            std::vector<record>::iterator it;
            for (it = schedTrace_.begin(); it < schedTrace_.end(); ++it) {
                myfile << (*it).cycle << " " << (*it).funct << "\n";
            }
            myfile.close();
        }
    }

#endif

    /*The following are virtual functions which
        the highest level schedulers must implement*/

    //The function which will actually run the scheduling procedure
    virtual void policy(void) = 0;
    //Tells the scheduler how to push a TP
    virtual bool placeTP(tpClosure* input) = 0;
    //Tell the scheduler how to push a Codelet
    virtual bool placeCodelet(Codelet* input) = 0;

    inline void setRuntimeTPScheds(ABCScheduler** TPSched_)
    {
        this->runtimeTPSchedulers = TPSched_;
    }

    inline ABCScheduler* getRuntimeTPSched(uint64_t num)
    {
        return this->runtimeTPSchedulers[num];
    }

    inline void setNumTPSched(uint64_t num)
    {
        this->numTPSched = num;
    }

    inline uint64_t getNumTPSched()
    {
        return this->numTPSched;
    }

    inline void setNumMCSched(uint64_t num)
    {
        this->numMCSched = num;
    }

    inline uint64_t getNumMCSched()
    {
        return this->numMCSched;
    }

    inline void setNumSched(uint64_t num)
    {
        this->numSched = num;
    }

    inline uint64_t getNumSched()
    {
        return this->numSched;
    }

    inline void setNumOfMaxUnitsPerCluster(uint64_t num)
    {
        this->numOfMaxUnitsPerCluster = num;
    }

    inline uint64_t getNumOfMaxUnitsPerCluster()
    {
        return this->numOfMaxUnitsPerCluster;
    }

}; //class
} //namespace
