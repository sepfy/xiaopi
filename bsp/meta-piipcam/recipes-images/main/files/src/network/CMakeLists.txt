cmake_minimum_required(VERSION 3.1)
project(network_manager)


option(UNITTEST_ENABLE "Build the unittest code" OFF)

file(GLOB LIB_SRC "*.cc")
file(GLOB UNITTEST_SRC "*unittest.cc*")
list(REMOVE_ITEM LIB_SRC ${UNITTEST_SRC})

include_directories(../)
include_directories(../../3rdparty/json/include)
include_directories(../3rdparty/pistache/include)
link_directories(../3rdparty/pistache/build/src)

if(UNITTEST_ENABLE)
  add_definitions(-DDEVEL)
endif(UNITTEST_ENABLE)

add_library(network_manager ${LIB_SRC})
target_link_libraries(network_manager system_manager utility wpa_client)

if(UNITTEST_ENABLE)
  foreach(sourcefile ${UNITTEST_SRC})
    string(REPLACE ".cc" "" appname ${sourcefile})
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" appname ${appname})
    add_executable(${appname} ${sourcefile})
    target_link_libraries(${appname} network_manager utility gtest pthread)
  endforeach(sourcefile ${UNITTEST_SRC})
endif(UNITTEST_ENABLE)
