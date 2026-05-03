cmake_minimum_required(VERSION 3.20)
project(Tuna2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-O1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Tuna2 main.cpp
        tt.h
        search.h
        uci.h
        constants.h
        root.h
        perft.h
        chess.hpp
        evaluation.h
        move_ordering_testing.h
        texel.h
)

