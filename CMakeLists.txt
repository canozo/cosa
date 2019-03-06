cmake_minimum_required(VERSION 3.1)

project(
  preprocessor
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")

include_directories(include/)

add_executable(
  preprocessor
  src/main.cpp
)
