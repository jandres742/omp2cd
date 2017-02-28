add_executable(is.darts.exe is.output.darts.cpp wtime.output.darts.cpp c_timers.output.darts.cpp c_print_results.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(is.darts.exe darts)

include_directories(../../darts-lib)
