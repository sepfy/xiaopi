cmake_minimum_required(VERSION 3.1)
project(media)

include_directories(../)

file(GLOB LIB_SRC "*.cc")
add_library(media ${LIB_SRC})

