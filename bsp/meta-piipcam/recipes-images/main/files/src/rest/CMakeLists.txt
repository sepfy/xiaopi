cmake_minimum_required(VERSION 3.1)
project(rest)

include_directories(../../3rdparty/json/include)
include_directories(../../3rdparty/pistache/include)
include_directories(../)
include_directories(./)

file(GLOB APP_SOURCES "*.cc")
add_library(rest ${APP_SOURCES})
target_link_libraries(rest libpistache.a pthread)
