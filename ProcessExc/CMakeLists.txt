cmake_minimum_required(VERSION 3.8)
project(ProcessExc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ProcessControl.h ForkTest.h)
add_executable(ProcessExc ${SOURCE_FILES})