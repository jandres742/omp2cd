add_executable(graph500.darts.exe omp-csr/omp-csr.output.darts.cpp graph500.output.darts.cpp options.cpp rmat.cpp kronecker.cpp verify.cpp prng.cpp xalloc.cpp timer.cpp generator/splittable_mrg.cpp generator/graph_generator.cpp generator/make_graph.cpp generator/utils.cpp ../../darts-lib/utils.cpp)
target_link_libraries(graph500.darts.exe rt m darts)

include_directories(../../darts-lib)
