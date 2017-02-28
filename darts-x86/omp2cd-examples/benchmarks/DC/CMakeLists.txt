add_executable(dc.darts.exe adc.output.darts.cpp dc.output.darts.cpp extbuild.output.darts.cpp rbt.output.darts.cpp jobcntl.output.darts.cpp c_print_results.output.darts.cpp c_timers.output.darts.cpp wtime.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(dc.darts.exe darts)

include_directories(../../darts-lib)
