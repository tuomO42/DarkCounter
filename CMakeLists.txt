cmake_minimum_required(VERSION 3.22.1)
project(DarkCounter VERSION 0.0.1)

SET(CMAKE_CXX_STANDARD 20) 
SET(CMAKE_CXX_STANDARD_REQUIRED)

add_executable(DarkCounter main.cpp lib/observer.cpp lib/observer_factory.cpp)
