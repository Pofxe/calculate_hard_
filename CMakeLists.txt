# Устанавливаем минимальную версию CMake, необходимую для сборки проекта
cmake_minimum_required(VERSION 3.10)

# Устанавливаем имя проекта
project(calculate_hard_)

# Указываем, что проект собирается как исполняемый файл
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исходные файлы проекта
set(SOURCES
    src/main.cpp
)
set(HEADERS
    src/Token.h
)

# Добавляем исполняемый файл
add_executable(calculate ${SOURCES} ${HEADERS})