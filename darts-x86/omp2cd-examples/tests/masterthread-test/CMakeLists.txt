add_executable(masterthread.darts.exe masterthread.output.darts.cpp  ../../darts-lib/utils.cpp)
target_link_libraries(masterthread.darts.exe darts)

include_directories(../../darts-lib)
