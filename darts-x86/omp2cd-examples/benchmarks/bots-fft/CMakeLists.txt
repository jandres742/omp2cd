add_executable(bots-fft.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp fft.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-fft.darts.exe darts)

include_directories(../../darts-lib)
