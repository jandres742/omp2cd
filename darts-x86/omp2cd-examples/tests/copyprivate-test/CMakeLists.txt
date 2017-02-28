add_executable(copyprivate.darts.exe copyprivate.output.darts.cpp  ../../darts-lib/utils.cpp)
target_link_libraries(copyprivate.darts.exe darts)

include_directories(../../darts-lib)
