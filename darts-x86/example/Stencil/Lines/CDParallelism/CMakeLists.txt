set (stencil_src_files
Compute.cpp
Stencil.cpp
Swap.cpp
utils.cpp
main.cpp
)

add_executable (StencilLinesCD ${stencil_src_files})
target_link_libraries (StencilLinesCD darts)
