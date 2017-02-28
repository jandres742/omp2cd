add_executable(XSBench.darts.exe Main.output.darts.cpp CalculateXS.output.darts.cpp GridInit.output.darts.cpp XSutils.output.darts.cpp Materials.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(XSBench.darts.exe darts)

include_directories(../../darts-lib)
