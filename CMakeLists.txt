cmake_minimum_required(VERSION 3.16)
project(cRPC)

set(CMAKE_CXX_STANDARD 20)

find_package(msgpack REQUIRED)

#add_executable(cRPC main.cpp)
include_directories(include dependencies/include)
add_subdirectory(lib)
add_subdirectory(examples)