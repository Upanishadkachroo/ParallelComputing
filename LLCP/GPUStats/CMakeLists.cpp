cmake_minimum_required(VERSION 3.18)

project(
GPUStatistics
LANGUAGES
CXX
CUDA
)

set(
CMAKE_CXX_STANDARD
17
)

include_directories(
include
cuda
)

add_executable(

GPUStatistics

main.cpp

src/Statistics.cpp

cuda/StatisticsCUDA.cu

)

set_target_properties(
GPUStatistics
PROPERTIES
CUDA_SEPARABLE_COMPILATION ON
)