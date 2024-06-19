cmake_minimum_required(VERSION 3.0.0)


add_subdirectory(src)
add_subdirectory(test)

enable_testing()
include(CTest)


# Tau requires at least C11/C++11
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    Tau
    URL https://github.com/jasmcaus/tau/archive/dev.zip
)

FetchContent_MakeAvailable(Tau)