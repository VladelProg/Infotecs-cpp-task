cmake_minimum_required(VERSION 3.10)
project(lib)

set(CMAKE_CXX_STANDARD 14)

add_library(lib SHARED src/lib.cpp)

target_include_directories(lib PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)
