cmake_minimum_required(VERSION 3.16)

project(cpp-cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp-cache
    src/main.cpp
    src/Cache.cpp
)

target_include_directories(cpp-cache PRIVATE include)