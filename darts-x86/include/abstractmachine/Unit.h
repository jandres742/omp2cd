#ifndef DARTS_HWLOC_UNIT_H
#define DARTS_HWLOC_UNIT_H
namespace darts {
namespace hwloc {
    /**
		 * \brief Simple, "read-only" data structure: hardware units are immutable
		 * XXX FIXME TODO Move this code to its own file
		 */
    class Unit {
    private:
        uint64_t _cluster, _localMem, _id;

    public:
        Unit()
        {
        }

        Unit(const uint64_t cluster, const uint64_t localMem, const uint64_t id)
            : _cluster(cluster)
            , _localMem(localMem)
            , _id(id)
        {
        }
        /** \brief @return the cluster ID */
        uint64_t getCluster() const
        {
            return _cluster;
        }
        /** \brief @return the local memory ID */
        uint64_t getLocalMem() const
        {
            return _localMem;
        }
        /** \brief @return the core ID */
        uint64_t getId() const
        {
            return _id;
        }
        /**
			 * \brief pins the calling thread to this HW unit.
			 */
        void pin()
        {
            cpu_set_t mask;
            CPU_ZERO(&mask);
            CPU_SET(_id, &mask);
            if (0 != sched_setaffinity(0, sizeof(mask), &mask))
                perror("sched_setaffinity");
        }
    };
}
}

#endif
