set(runtime_lib_src
    Runtime.cpp
    )

set(runtime_inc
    ${CMAKE_SOURCE_DIR}/include/runtime/Runtime.h
    ${CMAKE_SOURCE_DIR}/include/runtime/CodeletFinal.h)

add_library(darts STATIC ${runtime_lib_src} ${runtime_inc})
target_link_libraries(darts common amm rsmanager codelet scheduler threadlocal rt)

if ( TBB_FOUND )
    target_link_libraries( darts ${TBB_LIBRARIES} )
endif ( TBB_FOUND )

set_target_properties(darts PROPERTIES PUBLIC_HEADER 
"${CMAKE_SOURCE_DIR}/include/runtime/Runtime.h;${CMAKE_SOURCE_DIR}/include/runtime/CodeletFinal.h")

install(TARGETS darts 
    EXPORT dartsLibraryDepends
    ARCHIVE DESTINATION darts/lib/           COMPONENT bin
    PUBLIC_HEADER DESTINATION darts/include/ COMPONENT dev)
