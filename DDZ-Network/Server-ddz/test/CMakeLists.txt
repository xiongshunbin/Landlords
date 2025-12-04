cmake_minimum_required(VERSION 3.22)
project(Test)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(./ SRC)

add_library(test ${SRC})
target_link_libraries(test protobuf)
