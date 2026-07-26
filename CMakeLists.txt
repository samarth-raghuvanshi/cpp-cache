cmake_minimum_required(VERSION 3.20)

project(cpp-cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(cache_core
    src/Cache.cpp
    src/CacheSerializer.cpp
    src/CommandParser.cpp
)

add_executable(cli
    apps/cli_main.cpp
)

target_link_libraries(cli
    PRIVATE cache_core
)

add_executable(server
    apps/server_main.cpp
    src/Server.cpp
)

target_link_libraries(server
    PRIVATE
        cache_core
        ws2_32
)

add_executable(client
    apps/client_main.cpp
    src/Client.cpp
)

target_link_libraries(client
    PRIVATE
        cache_core
        ws2_32
)

add_executable(benchmark
    apps/benchmark_main.cpp
)

target_link_libraries(benchmark
    PRIVATE
        cache_core
)