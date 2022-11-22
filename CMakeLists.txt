cmake_minimum_required(VERSION 3.23)
set(CMAKE_C_STANDARD 99)

project(PE_A12 C)
add_executable(PE_A12 src/HFProgTeilA12.c src/hashmap.c src/include/hashmap.h src/include/list.h src/list.c)

project(PE_A3 C)
add_executable(PE_A3 src/HFProgTeilA3.c)
