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
include_directories(${Boost_INCLUDE_DIR} ./src/)

add_executable(queue src/main.cpp src/server.cpp src/server.hpp src/session.cpp src/session.hpp src/common/message.hpp src/common/message.cpp src/common/message_queue.hpp src/topic.hpp src/topic.cpp)
TARGET_LINK_LIBRARIES(queue ${Boost_LIBRARIES} pthread)

add_executable(producer src/client/producer.cpp src/common/message.hpp src/common/message.cpp)
TARGET_LINK_LIBRARIES(producer ${Boost_LIBRARIES} pthread)

add_executable(consumer src/client/consumer.cpp src/common/message.hpp src/common/message.cpp)
TARGET_LINK_LIBRARIES(consumer ${Boost_LIBRARIES} pthread)
