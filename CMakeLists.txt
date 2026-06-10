cmake_minimum_required(VERSION 3.14)
project(DataStructure VERSION 1.0.0 LANGUAGES CXX)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 头文件路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 可选：构建测试
option(BUILD_TESTS "Build test programs" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
