cmake_minimum_required(VERSION 4.0)
project(os_practice_c C)

set(CMAKE_C_STANDARD 17)

add_executable(copy ch04-file-io/practice.c)
add_executable(os_practice_c main.c)
add_executable(race_hazard ch05-file-io-indepth/race_hazard.c)
add_executable(non_block ch05-file-io-indepth/non_block.c)
add_executable(proc ch06-processes/proc.c)
add_executable(pipes ch44-pipes_and_fifo/pipes.c)
add_executable(forwarder port-forwarder/forwarder.c port-forwarder/forwarder.h)
