cmake_minimum_required(VERSION 3.8)
project(s6fresnel)

set(CMAKE_CXX_STANDARD 11)

enable_testing ()
add_subdirectory (test)

add_test (NAME MyTest COMMAND s6test)