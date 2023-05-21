cmake_minimum_required(VERSION 3.0)
project(common_test)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/../lib)

link_directories(${PROJECT_SOURCE_DIR}/../lib)


set(SRC_LIST read_test.cc)
add_executable(rshmtest ${SRC_LIST})
target_link_libraries(rshmtest shm pthread)

set(SRC_LIST write_test.cc)
add_executable(wshmtest ${SRC_LIST})
target_link_libraries(wshmtest shm pthread)

set(SRC_LIST delshm_test.cc)
add_executable(dshmtest ${SRC_LIST})
target_link_libraries(dshmtest shm pthread)