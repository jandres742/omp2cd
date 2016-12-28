set( threadlocal_src 
     threadlocal.cpp
    )
set( threadlocal_inc
    ${CMAKE_SOURCE_DIR}/include/threadlocal/threadlocal.h
    )
    
add_library( threadlocal STATIC ${threadlocal_src} ${threadlocal_inc} )

set_target_properties(threadlocal PROPERTIES PUBLIC_HEADER 
"${CMAKE_SOURCE_DIR}/include/threadlocal/threadlocal.h")

install(TARGETS threadlocal 
    EXPORT dartsLibraryDepends
    ARCHIVE DESTINATION darts/lib/           COMPONENT bin
    PUBLIC_HEADER DESTINATION darts/include/ COMPONENT dev)
