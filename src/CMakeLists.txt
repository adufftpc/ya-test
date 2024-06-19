cmake_minimum_required(VERSION 3.0.0)

project(ya-test VERSION 0.0.1 LANGUAGES C CXX)
set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} main.cpp)

target_include_directories(${EXECUTABLE} PRIVATE
    ../include/
)
