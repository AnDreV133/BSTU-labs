cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(S_FUNCTION sources/function)
set(H_FUNCTION headers/function)

set(HEADER
${H_FUNCTION}/Function.h 
${H_FUNCTION}/FunctionManager.h
${H_FUNCTION}/Solver.h
${H_FUNCTION}/IOHandler.h
${H_FUNCTION}/specific/Hyperbola.h
${H_FUNCTION}/specific/Line.h
${H_FUNCTION}/specific/Parabola.h
)

set(SOURCE 
${S_FUNCTION}/Function.cpp 
${S_FUNCTION}/FunctionManager.cpp
${S_FUNCTION}/Solver.cpp
${S_FUNCTION}/IOHandler.cpp
${S_FUNCTION}/specific/Hyperbola.cpp
${S_FUNCTION}/specific/Line.cpp
${S_FUNCTION}/specific/Parabola.cpp
)

add_executable(project 
${HEADER}
${SOURCE}
main.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
