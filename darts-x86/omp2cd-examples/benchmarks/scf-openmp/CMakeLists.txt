add_executable(scf.darts.exe scf.output.darts.cpp twoel.output.darts.cpp integ.output.darts.cpp timer.output.darts.cpp output.output.darts.cpp diagonalize.output.darts.cpp input.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(scf.darts.exe darts)

include_directories(../../darts-lib)
