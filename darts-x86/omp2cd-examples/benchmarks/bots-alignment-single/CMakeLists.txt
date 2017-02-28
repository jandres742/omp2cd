add_executable(bots-alignment-single.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp sequence.output.darts.cpp alignment.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-alignment-single.darts.exe darts)

include_directories(../../darts-lib)
