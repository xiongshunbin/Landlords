cmake_minimum_required(VERSION 3.22)
project(Test)

set(CMAKE_CXX_STANDARD 20)

# 生成的代码中还需要使用到protobuf包
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

aux_source_directory(./ SRC)

add_library(test ${SRC})
target_link_libraries(test protobuf)
