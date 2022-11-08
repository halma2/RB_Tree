cmake_minimum_required(VERSION 3.14)
project(RB_Tree)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic")

add_executable(rb_tree src/main.cpp)

target_include_directories(rb_tree PRIVATE include)
