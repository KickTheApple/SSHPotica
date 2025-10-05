cmake_minimum_required(VERSION 4.0)
project(SSHPotica C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(SSHPotica
        main.c
        main.h)
