cmake_minimum_required(VERSION 3.1)
project(RDBMS)

set(CMAKE_CXX_STANDARD 14)
add_library(headers "headers.cpp")
add_executable(rdbms ${CMAKE_SOURCE_DIR}/rdbms.cpp)
target_link_libraries(rdbms headers)