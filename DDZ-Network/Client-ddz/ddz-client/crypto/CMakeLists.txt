cmake_minimum_required(VERSION 3.16)

project(crypto)

# 取出当前目录下的所有头文件
set(HEADERS
    AesCrypto.h
)

# 取出当前目录下的所有源文件
set(SRC
    AesCrypto.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(crypto ${SRC} ${HEADERS})

target_link_libraries(crypto PRIVATE Qt${QT_VERSION_MAJOR}::Core
    "C:/OpenSSL-Win64/lib/libcrypto.lib")
