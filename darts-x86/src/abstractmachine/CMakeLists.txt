set( amm_src AbstractMachine.cpp Affinity.cpp )
set( amm_inc 
    ${CMAKE_SOURCE_DIR}/include/abstractmachine/AbstractMachine.h 
    ${CMAKE_SOURCE_DIR}/include/abstractmachine/Cluster.h 
    ${CMAKE_SOURCE_DIR}/include/abstractmachine/Unit.h
    ${CMAKE_SOURCE_DIR}/include/abstractmachine/Affinity.h)

add_library( amm STATIC ${amm_src} ${amm_inc})

target_link_libraries( amm ${HWLOC_LIBRARIES} )

set_target_properties(amm PROPERTIES PUBLIC_HEADER
    "${CMAKE_SOURCE_DIR}/include/abstractmachine/AbstractMachine.h;${CMAKE_SOURCE_DIR}/include/abstractmachine/Cluster.h;${CMAKE_SOURCE_DIR}/include/abstractmachine/Unit.h;${CMAKE_SOURCE_DIR}/include/abstractmachine/Affinity.h")

install(TARGETS amm     
    EXPORT dartsLibraryDepends
    ARCHIVE DESTINATION darts/lib/           COMPONENT bin
    PUBLIC_HEADER DESTINATION darts/include/ COMPONENT dev)
