find_path(HWLOC_INCLUDE_DIR "hwloc.h" 
  HINTS ${HWLOC_ROOT}/include
  /usr/include
  /usr/local/include
  /opt/local/include
  /opt/hwloc/include
  ~/local/opt/include
  /home/1388/downloads/hwloc-1.11.2/include
)

find_library(HWLOC_LIBRARIES NAMES hwloc 
  HINTS ${HWLOC_ROOT}/lib
  PATHS /usr/lib /usr/local/lib /opt/local/lib /opt/hwloc/lib ~/local/opt/lib ~/bin/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HWLOC DEFAULT_MSG HWLOC_LIBRARIES
  HWLOC_INCLUDE_DIR)

if(HWLOC_FOUND)
  message(STATUS "Found HWLOC in ${HWLOC_INCLUDE_DIR};${HWLOC_LIBRARIES}")
  set( HWLOC_INCLUDE_DIR ${HWLOC_INCLUDE_DIR} )
  set( HWLOC_LIBRARIES ${HWLOC_LIBRARIES} )
endif(HWLOC_FOUND)
