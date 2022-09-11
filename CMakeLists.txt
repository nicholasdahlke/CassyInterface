cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
project(CassyInterface)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(hidapi)
find_package(wxWidgets CONFIG REQUIRED)


target_link_libraries(CassyInterface PRIVATE hidapi::hidapi wx::core wx::base)