cmake_minimum_required(VERSION 3.10)

project(calculate_hard_)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
)
set(HEADERS
    src/Token.h
    src/Sorter.h
    src/Calculate.h
)

add_executable(calculate ${SOURCES} ${HEADERS})