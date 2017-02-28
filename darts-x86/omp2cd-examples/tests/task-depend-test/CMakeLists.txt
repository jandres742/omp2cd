add_executable(task-depend-test.darts.exe task-depend-test.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(task-depend-test.darts.exe darts)

include_directories(../../darts-lib)
