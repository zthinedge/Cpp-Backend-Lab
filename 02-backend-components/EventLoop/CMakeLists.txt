cmake_minimum_required(VERSION 3.16)
project(EventLoopExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(test_eventloop
    Channel.cpp
    EventLoop.cpp
    Poller.cpp
    test_eventloop.cpp
)

target_compile_options(test_eventloop PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(test_eventloop PRIVATE pthread)
