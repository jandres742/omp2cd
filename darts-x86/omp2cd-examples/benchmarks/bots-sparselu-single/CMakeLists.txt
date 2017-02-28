add_executable(bots-sparselu-single.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp sparselu.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-sparselu-single.darts.exe darts)

include_directories(../../darts-lib)
