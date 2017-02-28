add_executable(bots-strassen.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp strassen.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-strassen.darts.exe darts)

include_directories(../../darts-lib)
