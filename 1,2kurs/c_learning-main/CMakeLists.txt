cmake_minimum_required(VERSION 3.23)
project(project C)

set(CMAKE_C_STANDARD 11)

add_executable(
        project
        main.c
        test_list/TestList.c
        test_list/TestList.h
#                CODEFORCE.c
)

add_library(
        Matrix
        libs/OperationOverMatrix.c
        libs/OperationOverMatrix.h
)

add_library(
        Set
        libs/OperationOverSet.c
        libs/OperationOverSet.h
)

add_library(
        String
        libs/String_DAA_lib.c
        libs/String_DAA_lib.h
)

add_library(
        Sorts
        libs/Sorts.c
        libs/Sorts.h
)

add_library(
        Vector
        libs/Vector.h
        libs/Vector.c
)

target_link_libraries(
        project
        Matrix
        Set
        String
        Sorts
        Vector
)