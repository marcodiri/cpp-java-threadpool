cmake_minimum_required(VERSION 3.19)
project(PC_cpp_java_threadpool)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES main.cpp ThreadPool/FixedThreadPool.h ThreadPool/FixedThreadPool.cpp ThreadPool/Runnable.h)
add_executable(PC_cpp_java_threadpool ${SOURCE_FILES})