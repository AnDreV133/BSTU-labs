cmake_minimum_required(VERSION 3.25)
project(lab_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(
        lab_2_colored_square
        Frame.h
        main.cpp
        Painter.h
        Assets.h
        interpolator/BarycentricInterpolator.h
        interpolator/RadialInterpolator.h
        interpolator/SectorInterpolator.h
        interpolator/BaseInterpolator.h global_values.h interpolator/OneColorInterpolator.h interpolator/RadialMultiColorInterpolator.h Matrix.h SawShader.h)
