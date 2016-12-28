set(common_src
    Atomics.cpp
    Lock.cpp
    Thread.cpp
    getClock.cpp
	Timer.cpp)

set(common_inc
    ${CMAKE_SOURCE_DIR}/include/common/Atomics.h
    ${CMAKE_SOURCE_DIR}/include/common/AutoLock.h
    ${CMAKE_SOURCE_DIR}/include/common/darts.h
    ${CMAKE_SOURCE_DIR}/include/common/getClock.h
    ${CMAKE_SOURCE_DIR}/include/common/Lock.h
    ${CMAKE_SOURCE_DIR}/include/common/rdtsc.h
    ${CMAKE_SOURCE_DIR}/include/common/Thread.h
    ${CMAKE_SOURCE_DIR}/include/common/ringbuffer.h
	${CMAKE_SOURCE_DIR}/include/common/Timer.h)

add_library(common STATIC ${common_src} ${common_inc})
target_link_libraries(common ${CMAKE_THREAD_LIBS_INIT} )

set_target_properties(common PROPERTIES PUBLIC_HEADER
"${CMAKE_SOURCE_DIR}/include/common/Atomics.h;${CMAKE_SOURCE_DIR}/include/common/AutoLock.h;${CMAKE_SOURCE_DIR}/include/common/darts.h;${CMAKE_SOURCE_DIR}/include/common/getClock.h;${CMAKE_SOURCE_DIR}/include/common/Lock.h;${CMAKE_SOURCE_DIR}/include/common/rdtsc.h;${CMAKE_SOURCE_DIR}/include/common/Thread.h;${CMAKE_SOURCE_DIR}/include/common/ringbuffer.h")

install(TARGETS common 
    EXPORT dartsLibraryDepends
    ARCHIVE DESTINATION darts/lib/           COMPONENT bin
    PUBLIC_HEADER DESTINATION darts/include/ COMPONENT dev)

