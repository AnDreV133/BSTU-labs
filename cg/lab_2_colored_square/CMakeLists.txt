cmake_minimum_required(VERSION 3.25)
project(lab_2_colored_square)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab_2_colored_square
        Frame.h
        main.cpp
        Painter.h Assets.h)
