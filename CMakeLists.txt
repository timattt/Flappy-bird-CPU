cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(FB)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")

add_library(math math/Math.cpp)
add_library(engine engine/Engine.cpp)
add_executable(core core/Game.cpp core/Utils.cpp)

target_link_libraries(core math engine)