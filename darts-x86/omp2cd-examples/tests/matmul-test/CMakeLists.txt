add_executable(matmul.darts.exe matmul.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(matmul.darts.exe darts)

include_directories(../../darts-lib)
