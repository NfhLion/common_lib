# 设置cmake的最低版本和项目名称
cmake_minimum_required(VERSION 3.0)
project(common)

# 生产debug版本
set(CMAKE_BUILD_TYPE "Debug")

# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/common/net)

add_subdirectory(common)