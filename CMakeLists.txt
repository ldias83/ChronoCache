cmake_minimum_required(VERSION 3.16)
project(HelloWorld LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_executable(chronocache src/main.cpp)

target_compile_options(chronocache PRIVATE -Wall -Wextra)
target_link_libraries(chronocache pthread)

# Benchmark binary
find_package(benchmark REQUIRED)

add_executable(bench_chronocache bmk/benchmark_chronocache.cpp)
target_link_libraries(bench_chronocache benchmark::benchmark pthread)
target_include_directories(bench_chronocache PRIVATE inc)