cmake_minimum_required(VERSION 3.12)

project(
        SandPile
        VERSION 1.0
)

set(CMAKE_CXX_STANDARD 17)

add_executable(SandPile main.cpp bmp.h bmp.cpp Parser.h Parser.cpp sandpile.h sandpile.cpp)
