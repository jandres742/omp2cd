add_executable(parallelfor.darts.exe parallelfor.output.darts.cpp  ../../darts-lib/utils.cpp)
target_link_libraries(parallelfor.darts.exe darts)

include_directories(../../darts-lib)
