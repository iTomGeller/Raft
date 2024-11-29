cmake_minimum_required(VERSION 3.10)

project(TestProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定头文件的搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/asio/include)  # ASIO 头文件路径

# 指定源文件的目录
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# 添加可执行文件
add_executable(test ${SOURCES})