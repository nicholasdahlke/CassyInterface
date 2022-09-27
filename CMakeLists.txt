cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
project(CassyInterface)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(hidapi)

add_executable(CassyInterface main.cpp Cassy.h Cassy.cpp)

target_link_libraries(CassyInterface PRIVATE hidapi::hidapi)