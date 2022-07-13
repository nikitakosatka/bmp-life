cmake_minimum_required(VERSION 3.21.2)
project(bmp_life C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(bmp_life
        main.c
        utils.c
        utils.h)
