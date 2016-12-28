set(Source_files
fib-o2d.cpp
theTPs.cpp
theCodelets.cpp
)

add_executable(fib-o2d ${Source_files})
target_link_libraries(fib-o2d darts)

