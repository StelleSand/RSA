cmake_minimum_required(VERSION 3.5)
project(RSA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp RSA.cpp RSA.h HighPrecisionDigit.cpp HighPrecisionDigit.h)
add_executable(RSA ${SOURCE_FILES})