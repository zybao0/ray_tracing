cmake_minimum_required (VERSION 2.6)
project(ray_tracing)

include_directories(D:/project/ray_tracing/header)
link_directories(D:/project/ray_tracing/lib)

include_directories(D:/project/BMPImage/build)
link_directories(D:/project/BMPImage/build)

SET(CMAKE_CXX_FLAGS "-O2 -std=c++17")

add_executable(ray_tracing main.cpp) # CMAKE_CURRENT_SOURCE_DIR is implicit here
target_link_libraries(ray_tracing BMPImage ray objects)