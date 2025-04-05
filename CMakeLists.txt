cmake_minimum_required(VERSION 3.24)
project(htcw_pool VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(htcw_pool INTERFACE)
target_include_directories(htcw_pool INTERFACE
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
)
