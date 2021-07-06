cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(FB)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")

add_executable(FB core/Game.cpp core/Utils.cpp engine/Engine.cpp math/Math.cpp)

target_link_libraries(FB gdi32)

file(COPY "${PROJECT_SOURCE_DIR}/Resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")