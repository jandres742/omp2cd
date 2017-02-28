add_executable(arraybench.darts.exe arraybench.output.darts.cpp common.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(arraybench.darts.exe darts)

include_directories(../../darts-lib)
