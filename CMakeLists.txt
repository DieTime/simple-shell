cmake_minimum_required(VERSION 3.13)
project(shell C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -fsanitize=address")

add_executable(shell main.c shell.c)