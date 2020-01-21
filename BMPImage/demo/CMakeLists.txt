cmake_minimum_required (VERSION 2.6)
project (demo)

include_directories(D:/project/BMPImage/build)
link_directories(D:/project/BMPImage/build)

SET(CMAKE_CXX_FLAGS "-O2 -std=c++11")

add_executable(demo main.cpp) # CMAKE_CURRENT_SOURCE_DIR is implicit here
target_link_libraries(demo BMPImage)