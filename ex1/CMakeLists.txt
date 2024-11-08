cmake_minimum_required(VERSION 3.21)
project(ex1_asmaa_ghrayeb C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ex1_asmaa_ghrayeb main.c tests.c cipher.c)
add_executable(e1 cipher.c)
add_executable(e2 tests.c)

