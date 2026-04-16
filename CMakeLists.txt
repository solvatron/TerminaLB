cmake_minimum_required(VERSION 3.10)

project(TerminaLB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

add_executable(TerminaLB src/main.cpp)
