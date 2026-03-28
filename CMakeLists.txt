cmake_minimum_required(VERSION 3.25)
project(Home C)
add_compile_options(-Wall -Wextra -pedantic)

add_library(graph STATIC src/graphStates/graphStates.c)

add_executable(main src/graphStates/main.c)
add_executable(graphTests src/graphStates/test.c)

target_link_libraries(graphTests PRIVATE graph)
target_link_libraries(main PRIVATE graph)
