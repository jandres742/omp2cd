add_executable(bots-uts.darts.exe bots_common.output.darts.cpp bots_main.output.darts.cpp brg_sha1.output.darts.cpp uts.output.darts.cpp ../../darts-lib/utils.cpp)
target_link_libraries(bots-uts.darts.exe darts)

include_directories(../../darts-lib)
