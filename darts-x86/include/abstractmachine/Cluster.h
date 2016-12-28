#ifndef DARTS_HWLOC_CLUSTER_H
#define DARTS_HWLOC_CLUSTER_H

#include <Unit.h>

namespace darts {
namespace hwloc {

    /**
     * \brief Cluster describes a cluster in our abstract machine. It is an immutable class.
     */
    class Cluster {
    private:
        /** \brief Cluster logical ID in the machine */
        uint64_t _id;

        /** \brief Cluster memory. For x86, it could be the last level of cache (LLC) */
        uint64_t _memId;

        /** \brief Number of units in this cluster */
        uint64_t _nbUnits;

        /** \brief units held in the cluster */
        Unit* _units;

    public:
        Cluster(uint64_t id = 0, uint64_t memId = 0, uint64_t nbUnits = 0, Unit* units = 0)
            : _id(id)
            , _memId(memId)
            , _nbUnits(nbUnits)
            , _units(units)
        {
        }
        ~Cluster()
        {
        }
        /** \brief Returns the ID of the cluster */ uint64_t getId() const
        {
            return _id;
        }
        /** \brief Returns the memory ID of the cluster */ uint64_t getMemId() const
        {
            return _memId;
        }
        /** \brief Returns the number of available units in the cluster */ uint64_t getNbUnits() const
        {
            return _nbUnits;
        }
        /** \brief Returns the array of units in the cluster */ Unit* getUnits() const
        {
            return _units;
        }
    };
}
}
#endif
