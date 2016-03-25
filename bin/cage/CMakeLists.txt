cmake_minimum_required(VERSION 2.8.9)

add_library (cage cage.cpp)

target_include_directories (cage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_cage test_cage.cpp)

