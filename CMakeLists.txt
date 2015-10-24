cmake_minimum_required(VERSION 3.3)
project(Statki)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES statki.cpp statki01.cpp statki02.cpp)
add_executable(Statki ${SOURCE_FILES})