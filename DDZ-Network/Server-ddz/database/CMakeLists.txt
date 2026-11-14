cmake_minimum_required(VERSION 3.22)
project(database)

set(CMAKE_CXX_STANDARD 20)

# 设置头文件所在路径
include_directories("/usr/include/mysql")

# 设置要加载的库路径
link_directories("/usr/lib/x86_64-linux-gnu/")

# 取出当前目录中所有的源文件, 存储到一个变量中
set(SRC
    MySQLConn.cpp
)

# 把源文件制作成静态库
add_library(database STATIC ${SRC})

# 设置要加载的库
target_link_libraries(database mysqlclient)
