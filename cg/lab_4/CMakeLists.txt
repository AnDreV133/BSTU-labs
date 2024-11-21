cmake_minimum_required(VERSION 3.25)
project(lab_4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab_4
        Frame.h
        main.cpp
        Matrix.h
        Painter.h
        Shape.h
        Vector.h Shader.h)
