add_executable(tasks.darts.exe tasks.output.darts.cpp  ../../darts-lib/utils.cpp)
target_link_libraries(tasks.darts.exe darts)

include_directories(../../darts-lib)
