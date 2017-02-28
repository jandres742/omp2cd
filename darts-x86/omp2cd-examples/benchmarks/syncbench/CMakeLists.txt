add_executable(syncbench.darts.exe syncbench.output.darts.cpp common.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(syncbench.darts.exe darts)

include_directories(../../darts-lib)
