add_executable(threadprivate.darts.exe threadprivate.output.darts.cpp  ../../darts-lib/utils.cpp)
target_link_libraries(threadprivate.darts.exe darts)

include_directories(../../darts-lib)
