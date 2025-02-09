cmake_minimum_required(VERSION 3.25)
project(lab8)

set(CMAKE_CXX_STANDARD 14)

# Указываем путь к папке с библиотекой
link_directories(${CMAKE_SOURCE_DIR})

# Добавляем исполняемый файл
add_executable(lab8 main.cpp)

# Подключаем библиотеку
target_link_libraries(lab8 PRIVATE lab2_c)