add_executable(ompcodeletclause.darts.exe ompcodeletclause.output.darts.cpp  ../../darts-lib/utils.cpp)
target_link_libraries(ompcodeletclause.darts.exe darts)

include_directories(../../darts-lib)
