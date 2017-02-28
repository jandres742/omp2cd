add_executable(fibtasks.darts.exe fibtasks.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(fibtasks.darts.exe darts)

include_directories(../../darts-lib)
