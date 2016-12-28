set (stencil_src_files
Compute.cpp
MatrixVector.cpp
End.cpp
utils.cpp
main.cpp
)

add_executable (MatrixVectorCD ${stencil_src_files})
target_link_libraries (MatrixVectorCD darts)
