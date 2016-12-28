set (stencil_src_files
Compute.cpp
Stencil.cpp
Swap.cpp
StencilWrapper.cpp
SwapWrapper.cpp
Launch.cpp
utils.cpp
main.cpp
)

add_executable (StencilLinesTP ${stencil_src_files})
target_link_libraries (StencilLinesTP darts)
