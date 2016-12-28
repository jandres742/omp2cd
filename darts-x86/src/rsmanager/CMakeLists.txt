set(rsmanager_src 
    Allocator.cpp
    HardwareSpecifier.cpp
    HardwareResource.cpp
    HeapAllocator.cpp
    HeapResource.cpp
    InformationService.cpp
    LimitSpecifier.cpp
    MasterResource.cpp
    MemoryAllocator.cpp
    MemoryResource.cpp
    NullResource.cpp
    Resource.cpp
    ResourceManager.cpp
    ShareSpecifier.cpp)

set(rsmanager_inc
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/Allocator.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/Defines.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/HardwareResource.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/HardwareSpecifier.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/HeapAllocator.h
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/HeapResource.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/InformationService.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/LimitSpecifier.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/MasterResource.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/MemoryAllocator.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/MemoryResource.h
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/NullAllocator.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/NullResource.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/Resource.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/ResourceManager.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/ResourceNeed.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/ShareSpecifier.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/Specifier.h 
    ${CMAKE_SOURCE_DIR}/include/resourcemanager/Typing.h)

add_library(rsmanager STATIC ${rsmanager_src} ${rsmanager_inc})
target_link_libraries(rsmanager common)

set_target_properties(rsmanager PROPERTIES PUBLIC_HEADER 
"${CMAKE_SOURCE_DIR}/include/resourcemanager/Allocator.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/Defines.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/HardwareResource.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/HardwareSpecifier.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/HeapAllocator.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/HeapResource.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/InformationService.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/LimitSpecifier.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/MasterResource.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/MemoryAllocator.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/MemoryResource.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/NullAllocator.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/NullResource.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/Resource.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/ResourceManager.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/ResourceNeed.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/ShareSpecifier.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/Specifier.h;${CMAKE_SOURCE_DIR}/include/resourcemanager/Typing.h")

install(TARGETS rsmanager 
    EXPORT dartsLibraryDepends
    ARCHIVE DESTINATION darts/lib/           COMPONENT bin
    PUBLIC_HEADER DESTINATION darts/include/ COMPONENT dev)
