cmake_minimum_required(VERSION 3.7)

project(HelloWorld LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CXX_FLAGS "-Wall -pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.66.0 REQUIRED COMPONENTS system regex thread)
find_path(Boost_INCLUDE_DIR boost)
include_directories(${Boost_INCLUDE_DIR})

add_executable(queuesystem src/main.cpp src/server.cpp src/server.hpp)
TARGET_LINK_LIBRARIES(queuesystem ${Boost_LIBRARIES} pthread)