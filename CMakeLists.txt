cmake_minimum_required(VERSION 3.16)
project(FileManager C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c vector.c file_system.c)
