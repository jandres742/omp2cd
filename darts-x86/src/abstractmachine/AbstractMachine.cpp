#include <AbstractMachine.h>

typedef hwloc_obj_t obj_t;

void darts::hwloc::AbstractMachine::discoverTopologyWithLLC(void)
{
    unsigned nbSockets = hwloc_get_nbobjs_by_type(_topology, HWLOC_OBJ_SOCKET);
    hwloc_obj_t o = hwloc_get_obj_by_type(_topology, HWLOC_OBJ_SOCKET, 0);

    hwloc_obj_t obj;
    for (obj = o->first_child;
         obj && obj->type != HWLOC_OBJ_CACHE;
         obj = obj->first_child)
        ;

    _nbClusters = nbSockets;
    if (obj) {
        int n = hwloc_get_nbobjs_inside_cpuset_by_type(_topology, obj->cpuset, HWLOC_OBJ_PU);
        _nbClusters = _nbTotalUnits / n; // XXX assumes homogeneous distribution of PUs
    }
    _clusterMap = new Cluster[_nbClusters];

    // TODO Refactor this code and the next function's code into a single one
    for (o = obj; o; o = o->next_cousin) {
        int nUnits = hwloc_get_nbobjs_inside_cpuset_by_type(_topology, o->cpuset, HWLOC_OBJ_PU);
        Unit* units = new Unit[nUnits];
        for (int i = 0; i < nUnits; ++i) {
            hwloc_obj_t t = hwloc_get_obj_inside_cpuset_by_type(_topology, o->cpuset, HWLOC_OBJ_PU, i);
            Unit hwu(o->logical_index, t->logical_index, t->os_index);
            units[i] = hwu; // simple shallow copy
        }
        Cluster cluster(o->logical_index, o->logical_index, nUnits, units);
        _clusterMap[o->logical_index] = cluster; // simple shallow copy
    }
}

void darts::hwloc::AbstractMachine::discoverTopology(void)
{
    _nbClusters = hwloc_get_nbobjs_by_type(_topology, HWLOC_OBJ_SOCKET);
    _clusterMap = new Cluster[_nbClusters];
    hwloc_obj_t o = hwloc_get_obj_by_type(_topology, HWLOC_OBJ_SOCKET, 0);
    // TODO Refactor this code and the previous function's code into a single one
    for (; o; o = o->next_cousin) {
        int nUnits = hwloc_get_nbobjs_inside_cpuset_by_type(_topology, o->cpuset, HWLOC_OBJ_PU);
        Unit* units = new Unit[nUnits];
        for (int i = 0; i < nUnits; ++i) {

            hwloc_obj_t t = hwloc_get_obj_inside_cpuset_by_type(_topology, o->cpuset, HWLOC_OBJ_PU, i);
            Unit hwu(o->logical_index, t->logical_index, t->os_index);
            units[i] = hwu; // simple shallow copy
        }
        Cluster cluster(o->logical_index, o->logical_index, nUnits, units);
        _clusterMap[o->logical_index] = cluster; // simple shallow copy
    }
}
