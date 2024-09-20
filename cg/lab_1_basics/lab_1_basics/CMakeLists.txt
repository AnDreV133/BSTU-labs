cmake_minimum_required(VERSION 3.25)
project(lab_1_basics)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(
        lab_1_basics
        Frame.h
        main.cpp
        Painter_Example.h Assets.h Painter_Var2.h
)
