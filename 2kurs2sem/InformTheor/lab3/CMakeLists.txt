cmake_minimum_required(VERSION 3.0.0)
project(lab3 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(lab3 
2.cpp 2.h 
3_1.cpp 3_1.h 
3_2.cpp 3_2.h
4.cpp 4.h
5.cpp 5.h
6.cpp 6.h
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
