cmake_minimum_required(VERSION 3.15)
project(optimization)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(lib)
include_directories(src)

file(GLOB SOURCES "src/*.cpp" "lib/*.cpp")

add_executable(project ${SOURCES})