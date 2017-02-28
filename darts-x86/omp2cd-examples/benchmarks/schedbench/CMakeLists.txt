add_executable(schedbench.darts.exe schedbench.output.darts.cpp common.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(schedbench.darts.exe darts)

include_directories(../../darts-lib)
