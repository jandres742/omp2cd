add_executable(critical-atomic.darts.exe critical-atomic.output.darts.cpp  ../../darts-lib/utils.cpp)
target_link_libraries(critical-atomic.darts.exe darts)

include_directories(../../darts-lib)
