cmake_minimum_required( VERSION 2.6 )

ENABLE_TESTING()

project ( darts )

set (CMAKE_C_COMPILER gcc)
set (CMAKE_CXX_COMPILER g++)

if( "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" )
message( "HOME" )
set (CMAKE_INSTALL_PREFIX $ENV{HOME} )
endif( "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" )
message("PREFIX = ${CMAKE_INSTALL_PREFIX}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Look for supported threading libraries
find_package( Threads REQUIRED )

if( UNIX )
    if( NOT CMAKE_USE_PTHREADS_INIT )
        message( FATAL_ERROR "Require pthreads on Unix-like systems" )
    endif( NOT CMAKE_USE_PTHREADS_INIT )
elseif( WIN32 )
    if( NOT CMAKE_USE_WIN32_THREADS_INIT )
        message( FATAL_ERROR "Require win32 threads on Windows systems" )
    endif( NOT CMAKE_USE_WIN32_THREADS_INIT )
else( WIN32 )
    message( FATAL_ERROR "Unsupported operating system" )
endif( UNIX )

find_package( Hwloc REQUIRED )
if( HWLOC_FOUND )
    include_directories( ${HWLOC_INCLUDE_DIR} )
    link_directories( ${HWLOC_LIBRARIES} )
endif( HWLOC_FOUND )

option(TBB "TBB" ON)
if(TBB)
find_package( TBB )
if ( TBB_FOUND )
    add_definitions( -DTBB )
    include_directories( ${TBB_INCLUDE_DIR} )
    link_directories( ${TBB_LIBRARY_DIRS} )
endif ( TBB_FOUND )
else()
message("DEQUE ON")
endif(TBB)

include_directories(include/abstractmachine
                    include/common 
                    include/resourcemanager
                    include/runtime
                    include/scheduler 
                    include/threading
                    include/threadlocal
                    include/threadsafe 
		    #Path to ATLAS include(optional)
		    #Path to boost libraries(optional)
)

option(DEBUG "DEBUG" OFF)
if(DEBUG)
message("DEBUG VERSION!!!")
set (CMAKE_BUILD_TYPE "DEBUG")
else()
message("Release Version")
set (CMAKE_BUILD_TYPE "RELEASE")
endif()


#Path to ATLAS lib
#LINK_DIRECTORIES(/opt/shared/atlas/3.10.2/lib /usr/lib )

#Select this for DEBUG
set (CMAKE_CXX_FLAGS_DEBUG "-g -std=c++0x -Wall -O3 -pg -lm") 

#Select this for RELEASE
set (CMAKE_CXX_FLAGS_RELEASE "-std=c++0x -Wall -O3 -lm -DNDEBUG")

#Select this for RELEASE if you have added the path to ATLAS
#set (CMAKE_CXX_FLAGS_RELEASE "-std=c++0x -Wall -O3 -lm -lsatlas -DNSDEBUG")

## Compiler flags
if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
    #set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -fpermissive" )
    set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fpermissive" )
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
    set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Werror " )
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC" )
    set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/W3 /wd4995" )
else()
    message( FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}" )
endif()
 
option(TRACE "TRACE" OFF)
if(TRACE)
message("Tracing ON!")
add_definitions(-DTRACE)
else()
message("Tracing off")
endif(TRACE)

FIND_LIBRARY(M_LIB m)
FIND_LIBRARY(RT_LIB rt)
FIND_LIBRARY(BLAS_LIB blas)

add_subdirectory( include )
add_subdirectory( src )
add_subdirectory( example )
add_subdirectory( omp2cd-examples )
add_subdirectory( performance )

export(TARGETS amm codelet common darts rsmanager scheduler threadlocal FILE "${PROJECT_BINARY_DIR}/dartsLibraryDepends.cmake")

export(PACKAGE darts)

configure_file(dartsConfig.cmake.in "${CMAKE_INSTALL_PREFIX}/darts/CMake/dartsConfig.cmake")

install(EXPORT dartsLibraryDepends DESTINATION darts/CMake/ COMPONENT dev)
