find_path(TBB_INCLUDE_DIR "tbb/tbb.h" 
  HINTS ${TBB_ROOT}/include
  /usr/include
  /usr/local/include
  /opt/local/include
  /opt/intel/tbb/include
  /lustre/work/capsl/opt/intel/tbb/include
  ~/local/tbb/include 
)

find_library(TBB_LIBRARIES NAMES tbb 
  HINTS ${TBB_ROOT}/lib
  /lustre/work/capsl/opt/intel/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21
  ~/local/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21
  /opt/intel/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21/
  PATHS /usr/lib /usr/local/lib /opt/local/lib /opt/intel/tbb/lib 
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TBB DEFAULT_MSG TBB_LIBRARIES
  TBB_INCLUDE_DIR)

if(TBB_FOUND)
  message(STATUS "Found TBB in ${TBB_INCLUDE_DIR};${TBB_LIBRARIES}")
  set( TBB_INCLUDE_DIR ${TBB_INCLUDE_DIR} )
  set( TBB_LIBRARIES ${TBB_LIBRARIES} )
endif(TBB_FOUND)
