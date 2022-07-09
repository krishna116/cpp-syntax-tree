cmake_minimum_required(VERSION 3.15)

project(cpp-syntax-tree VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

option(ENABLE_UNIT_TEST "enable unit test" TRUE)

configure_file(config.h.txt config.h)
add_subdirectory(src)

if(${ENABLE_UNIT_TEST})
    enable_testing()
    add_subdirectory(test)
endif()