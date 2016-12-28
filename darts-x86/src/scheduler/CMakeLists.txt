set(scheduler_sources
    TPSchedPolicy.cpp
    MSchedPolicy.cpp)

set(scheduler_inc
    ${CMAKE_SOURCE_DIR}/include/scheduler/ABCScheduler.h 
    ${CMAKE_SOURCE_DIR}/include/scheduler/MicroScheduler.h 
    ${CMAKE_SOURCE_DIR}/include/scheduler/MSchedPolicy.h 
    ${CMAKE_SOURCE_DIR}/include/scheduler/Scheduler.h 
    ${CMAKE_SOURCE_DIR}/include/scheduler/TPSchedPolicy.h 
    ${CMAKE_SOURCE_DIR}/include/scheduler/TPScheduler.h)

add_library(scheduler STATIC ${scheduler_sources} ${scheduler_inc})

set_target_properties(scheduler PROPERTIES PUBLIC_HEADER 
"${CMAKE_SOURCE_DIR}/include/scheduler/ABCScheduler.h;${CMAKE_SOURCE_DIR}/include/scheduler/MicroScheduler.h;${CMAKE_SOURCE_DIR}/include/scheduler/MSchedPolicy.h;${CMAKE_SOURCE_DIR}/include/scheduler/Scheduler.h;${CMAKE_SOURCE_DIR}/include/scheduler/TPSchedPolicy.h;${CMAKE_SOURCE_DIR}/include/scheduler/TPScheduler.h")

install(TARGETS scheduler     
    EXPORT dartsLibraryDepends
    ARCHIVE DESTINATION darts/lib/           COMPONENT bin
    PUBLIC_HEADER DESTINATION darts/include/ COMPONENT dev)


