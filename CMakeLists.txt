cmake_minimum_required(VERSION 3.1)

project(
  parser
  VERSION 1.0
  DESCRIPTION "Parsear datasets de JSON a texto simple"
  LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")

include_directories(include/)

add_executable(
  parser
  src/main.cpp
)
