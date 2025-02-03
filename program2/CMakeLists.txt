cmake_minimum_required(VERSION 3.10)
project(program2)

set(CMAKE_CXX_STANDARD 14)

add_executable(program2 src/program2.cpp)

target_link_libraries(program2 lib)
target_include_directories(program2 PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)
