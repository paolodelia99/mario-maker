cmake_minimum_required(VERSION 3.17)
project(mario_maker)

set(CMAKE_CXX_STANDARD 11)

file(GLOB src "src/*.cpp")
add_executable(mario_maker ${src})

target_link_libraries(mario_maker raylib)