set( codelet_src 
    Codelet.cpp 
    ThreadedProcedure.cpp
    )
set( codelet_inc
    ${CMAKE_SOURCE_DIR}/include/threading/Codelet.h 
    ${CMAKE_SOURCE_DIR}/include/threading/codeletDefines.h 
    ${CMAKE_SOURCE_DIR}/include/threading/SyncSlot.h 
    ${CMAKE_SOURCE_DIR}/include/threading/ThreadedProcedure.h
    ${CMAKE_SOURCE_DIR}/include/threading/doTP.h
    ${CMAKE_SOURCE_DIR}/include/threading/doLoop.h
    ${CMAKE_SOURCE_DIR}/include/threading/tpClosure.h
    ${CMAKE_SOURCE_DIR}/include/threading/loopClosure.h
    ${CMAKE_SOURCE_DIR}/include/threading/loop.h)
    
add_library( codelet STATIC ${codelet_src} ${codelet_inc} )
#target_link_libraries(codelet threadlocal)

set_target_properties(codelet PROPERTIES PUBLIC_HEADER 
"${CMAKE_SOURCE_DIR}/include/threading/Codelet.h;${CMAKE_SOURCE_DIR}/include/threading/codeletDefines.h;${CMAKE_SOURCE_DIR}/include/threading/SyncSlot.h;${CMAKE_SOURCE_DIR}/include/threading/ThreadedProcedure.h;${CMAKE_SOURCE_DIR}/include/threading/doTP.h;${CMAKE_SOURCE_DIR}/include/threading/doLoop.h;${CMAKE_SOURCE_DIR}/include/threading/tpClosure.h;${CMAKE_SOURCE_DIR}/include/threading/loopClosure.h;${CMAKE_SOURCE_DIR}/include/threading/loop.h")

install(TARGETS codelet 
    EXPORT dartsLibraryDepends
    ARCHIVE DESTINATION darts/lib/           COMPONENT bin
    PUBLIC_HEADER DESTINATION darts/include/ COMPONENT dev)
