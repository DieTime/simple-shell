cmake_minimum_required(VERSION 3.13)
project(shell C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-O2 -Wall -fsanitize=address -fsanitize=leak -g")

add_executable(shell main.c src/shell.c)