cmake_minimum_required(VERSION 3.13)
project(Parking_System)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-l sqlite3.c)
add_executable(Parking_System main.cpp sqlite3.h sqlite3.c)