#You must specify a minimum version of cmake to ensure correct behavior
cmake_minimum_required(VERSION 3.10)

#Set the project name
project(Day2)

#Enable support for a specific C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#This tells CMake to create an executable with the following name based on the specified source files
add_executable(main main.cpp)