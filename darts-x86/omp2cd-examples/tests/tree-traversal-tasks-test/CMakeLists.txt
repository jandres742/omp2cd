add_executable(tree-traversal-tasks.darts.exe tree-traversal-tasks.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(tree-traversal-tasks.darts.exe darts)

include_directories(../../darts-lib)
