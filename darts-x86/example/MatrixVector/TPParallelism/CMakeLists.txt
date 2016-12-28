set (MatrixVector_src_files
Compute.cpp
MatrixVector.cpp
Swap.cpp
MatrixVectorWrapper.cpp
SwapWrapper.cpp
Launch.cpp
utils.cpp
main.cpp
)

add_executable (MatrixVectorTP ${MatrixVector_src_files})
target_link_libraries (MatrixVectorTP darts)
