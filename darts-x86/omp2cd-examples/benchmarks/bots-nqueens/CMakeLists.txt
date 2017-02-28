add_executable(bots-nqueens.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp nqueens.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-nqueens.darts.exe darts)

include_directories(../../darts-lib)
