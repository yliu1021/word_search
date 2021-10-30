cmake_minimum_required(VERSION 3.20)
project(word_search)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

include(CTest)
add_subdirectory(tests)
