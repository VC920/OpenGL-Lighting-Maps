cmake_minimum_required(VERSION 3.10)
project(lighting-maps)
add_executable(${PROJECT_NAME} src/main.cpp src/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} SDL2 GL)