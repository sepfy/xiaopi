cmake_minimum_required(VERSION 3.1)
project(detection)

include_directories(../)

file(GLOB LIB_SRC "*.cc")
add_library(detection ${LIB_SRC})
target_link_libraries(detection utility)
