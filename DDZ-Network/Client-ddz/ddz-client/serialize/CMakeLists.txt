cmake_minimum_required(VERSION 3.16)

project(serialize)

include_directories("C:/Protobuf/include")

# 取出当前目录下的所有头文件
set(HEADERS
    Infomation.pb.h
    Codec.h
)

# 取出当前目录下的所有源文件
set(SRC
    Infomation.pb.cc
    Codec.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(serialize ${SRC} ${HEADERS})

target_link_libraries(serialize PRIVATE Qt${QT_VERSION_MAJOR}::Core)
