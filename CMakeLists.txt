cmake_minimum_required(VERSION 3.5)
project(snake LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} snake.cpp)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2main)
