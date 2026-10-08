cmake_minimum_required(VERSION 3.16)

project(tcp)

# 取出当前目录下的所有头文件
set(HEADERS
    TcpSocket.h
)

# 取出当前目录下的所有源文件
set(SRC
    TcpSocket.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(tcp ${SRC} ${HEADERS})

target_link_libraries(tcp PRIVATE Qt${QT_VERSION_MAJOR}::Core)
