cmake_minimum_required(VERSION 3.10)
project(program1)

set(CMAKE_CXX_STANDARD 14)

add_executable(program1 src/program1.cpp)

target_link_libraries(program1 lib)
target_include_directories(program1 PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)
