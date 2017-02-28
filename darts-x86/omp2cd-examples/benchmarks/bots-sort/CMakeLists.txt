add_executable(bots-sort.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp sort.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-sort.darts.exe darts)

include_directories(../../darts-lib)
