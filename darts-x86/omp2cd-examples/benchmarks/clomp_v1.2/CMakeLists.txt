add_executable(clomp.darts.exe clomp.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(clomp.darts.exe darts)

include_directories(../../darts-lib)
