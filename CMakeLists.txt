cmake_minimum_required(VERSION 3.10)
project(kv_store)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(kv_store src/main.cpp src/store.cpp src/server.cpp)
target_link_libraries(kv_store PRIVATE Threads::Threads)