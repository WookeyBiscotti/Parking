cmake_minimum_required(VERSION 3.10.0)
project(parking VERSION 0.1.0 LANGUAGES C CXX)

add_executable(parking 
src/main.cpp 
src/parking.cpp 
src/parking.hpp 
src/parking_interface.hpp
)

include(CTest)
enable_testing()

