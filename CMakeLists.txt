cmake_minimum_required(VERSION 4.0)
project(os_practice_c C)

set(CMAKE_C_STANDARD 17)

add_executable(copy ch04-file-io/practice.c)
add_executable(os_practice_c main.c)