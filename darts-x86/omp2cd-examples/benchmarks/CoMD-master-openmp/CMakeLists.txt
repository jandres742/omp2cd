add_executable(comd.darts.exe cmdLineParser.output.darts.cpp eam.output.darts.cpp linkCells.output.darts.cpp parallel.output.darts.cpp timestep.output.darts.cpp CoMD.output.darts.cpp haloExchange.output.darts.cpp ljForce.output.darts.cpp performanceTimers.output.darts.cpp yamlOutput.output.darts.cpp decomposition.output.darts.cpp initAtoms.output.darts.cpp mycommand.output.darts.cpp random.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(comd.darts.exe darts)

include_directories(../../darts-lib)
