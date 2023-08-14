cmake_minimum_required(VERSION 3.26.4)  
project(PR LANGUAGES)
set(CMAKE_CUDA_COMPILER /usr/local/cuda-12.2/bin/nvcc)
enable_language("CUDA")
enable_language(C)
add_executable(color_quantization projekt.cu typedefs.c orig.c)


find_package(PNG REQUIRED)
find_package(OpenMP)

include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(color_quantization ${PNG_LIBRARY})
target_link_libraries(color_quantization OpenMP::OpenMP_C)
target_link_libraries(color_quantization m)
set_target_properties(color_quantization PROPERTIES CUDA_SEPARABLE_COMPILATION ON)