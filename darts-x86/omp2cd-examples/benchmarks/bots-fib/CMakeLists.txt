add_executable(bots-fib.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp fib.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-fib.darts.exe darts)

include_directories(../../darts-lib)
