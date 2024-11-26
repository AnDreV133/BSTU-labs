cmake_minimum_required(VERSION 3.25)
project(lab_4_st)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab_4_st
        Frame.h
        main.cpp
        Matrix.h
        Painter.h
        shape.h
        Vector.h)
