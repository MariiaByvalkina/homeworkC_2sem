cmake_minimum_required(VERSION 3.25)
project(Home C)
add_compile_options(-Wall -Wextra -pedantic)

add_library(AVL STATIC src/airportsAVL/AVL.c)

add_executable(aerosoft src/airportsAVL/main.c)
add_executable(AVLtests src/airportsAVL/test.c)
target_link_libraries(AVLtests PRIVATE AVL)
target_link_libraries(aerosoft PRIVATE AVL)