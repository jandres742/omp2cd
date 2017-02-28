add_executable(lastprivate.darts.exe lastprivate.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(lastprivate.darts.exe darts)

include_directories(../../darts-lib)
