cmake_minimum_required(VERSION 3.1)
project(rest)

include_directories(../../3rdparty/json/include)
include_directories(../../3rdparty/pistache/include)
include_directories(../)
include_directories(./)

find_package(PkgConfig)
pkg_check_modules(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

file(GLOB APP_SOURCES "*.cc")
add_library(rest ${APP_SOURCES})
target_link_libraries(rest libpistache.a pthread)
