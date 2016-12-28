#ifndef HARDWARE_TOPOLOGY_HPP_V2
#define HARDWARE_TOPOLOGY_HPP_V2

#include <cassert>
#include <hwloc.h>
#include <stdint.h>

#include <Cluster.h>
#include <Unit.h>

// XXX TODO Move this code elsewhere

namespace darts {
namespace hwloc {

    /**
     * \brief Summarized hardware topology descriptor.
     * The current implementation wraps calls around the hwloc library to give
     * the core ids (Note: not the HW threads!).
     */
    class AbstractMachine {
    private:
        /** \TODO 
         * Use hwloc_topology_restrict() to make a "virtual block", ie in a 
         * multi-socket environment, give only the CPU set corresponding to a 
         * given socket/chip to a given scheduler.
         */

    public:
        hwloc_topology_t _topology;

    private:
        Cluster* _clusterMap;
        size_t _nbClusters;
        size_t _nbTotalUnits;

        /**
         * \brief Simple initialization of the topology description.
         */
        void
        init(bool useLLC)
        {
            _nbTotalUnits = hwloc_get_nbobjs_by_type(_topology, HWLOC_OBJ_PU);
            if (useLLC == false) {
                discoverTopology();
            } else {
                discoverTopologyWithLLC();
            }
        }
        /**
         * \brief Actual "intelligence" behind the class: iterates through cores
         * and collects them in a "map". Uses last level of cache as "cluster 
         * memory."
         * \FIXME The cache ID is the same for all L1/L2/L3 - "bug" from hwloc?
         * \TODO Map the right L1D cache to its core?
         * \TODO Provide different ways of partitioning the concrete machine in 
         * an abstract one
         */ void discoverTopologyWithLLC(void);

        /**
         * \brief Actual "intelligence" behind the class: iterates through cores
         * and collects them in a map.
         * \FIXME The cache ID is the same for all L1/L2/L3 - "bug" from hwloc?
         * \TODO Map the right L1D cache to its core?
         * \TODO Provide different ways of partitioning the concrete machine in 
         * an abstract one
         */
        void discoverTopology(void);

    public:
        AbstractMachine(bool useLLC = false)
            : _topology(0)
            , _clusterMap(0)
            , _nbClusters(0)
            , _nbTotalUnits(0)
        {
            hwloc_topology_init(&_topology);
            hwloc_topology_load(_topology);
            init(useLLC);
        }

        ~AbstractMachine()
        {
            for (size_t i = 0; i < _nbClusters; ++i) {
                Unit* u = _clusterMap[i].getUnits();
                delete[] u;
            }
            delete[] _clusterMap;
            hwloc_topology_destroy(_topology);
        }

        size_t getNbClusters() const
        {
            return _nbClusters;
        }
        Cluster* getClusterMap() const
        {
            return _clusterMap;
        }
        size_t getTotalNbUnits() const
        {
            return _nbTotalUnits;
        }
    };
} // hwloc
} // darts
#endif
