add_executable(reduce-omp.darts.exe reduce-omp.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(reduce-omp.darts.exe darts)

include_directories(../../darts-lib)
