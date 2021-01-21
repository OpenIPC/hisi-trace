cmake_minimum_required(VERSION 2.8.3)

project(hisi-trace CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${PRJ_SRCS})

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    )
