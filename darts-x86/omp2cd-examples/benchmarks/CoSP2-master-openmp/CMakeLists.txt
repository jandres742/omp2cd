add_executable(cosp2.darts.exe cmdLineParser.output.darts.cpp CoSP2.output.darts.cpp decomposition.output.darts.cpp generate.output.darts.cpp haloExchange.output.darts.cpp matrixio.output.darts.cpp mycommand.output.darts.cpp parallel.output.darts.cpp performance.output.darts.cpp random.output.darts.cpp sp2Loop.output.darts.cpp sparseMath.output.darts.cpp sparseMatrix.output.darts.cpp  ../../darts-lib/utils.cpp)

add_definitions(-DCONTIG_MATRIX)

target_link_libraries(cosp2.darts.exe darts)

include_directories(../../darts-lib)
