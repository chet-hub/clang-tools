cmake_minimum_required(VERSION 3.17)
project(ECS C)

set(CMAKE_C_STANDARD 99)

add_executable(ECS main.c ecs.h vector.h table.h hashmap.h)