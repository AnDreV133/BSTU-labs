cmake_minimum_required(VERSION 3.25)
project(lab_5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab_5
        Frame.h
        main.cpp
        Matrix.h
        Shape.h
        Vector.h Shader.h Painter.h)
