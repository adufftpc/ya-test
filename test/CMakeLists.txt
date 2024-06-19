cmake_minimum_required(VERSION 3.0.0)

project(ya-test-utests VERSION 0.0.1 LANGUAGES C CXX)
set(EXECUTABLE ${PROJECT_NAME}.elf)

enable_testing()

add_executable(${EXECUTABLE} tests.cpp)

target_link_libraries(${EXECUTABLE} Tau)

target_include_directories(${EXECUTABLE}  PRIVATE
    ../include/
)