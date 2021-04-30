cmake_minimum_required(VERSION 3.19)
project(PC_cpp_java_threadpool)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(ThreadPool_lib)
add_subdirectory(ThreadPool_lib)

target_link_libraries(${PROJECT_NAME} ThreadPool_lib)

add_subdirectory(GTests)