add_library(CDSerial CD_serial.cpp)

add_library(CDParallel CD_parallel.cpp)

add_library(CDSourceSinc CD_source_sinc.cpp)

add_library(TPParllel TP_parallel.cpp)

add_library(TPSerial TP_serial.cpp)

add_library(TPStrict TP_strict.cpp)

add_library(TPNonStrict TP_non_strict.cpp)

add_library(TPCDOverhead TP_CD_overhead.cpp)

add_library(Init init.cpp)

add_library(ParallelLoop Para_loop.cpp)

add_library(SerialLoop Serial_loop.cpp)

add_library(CodeletLoop Codelet_loop.cpp)

add_executable(microBench microBenchLauncher.cpp)
target_link_libraries(microBench darts CDSerial CDParallel CDSourceSinc TPParllel TPSerial TPStrict TPNonStrict TPCDOverhead Init ParallelLoop SerialLoop CodeletLoop)
