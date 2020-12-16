cmake_minimum_required(VERSION 3.1)
project(utility)


option(UNITTEST_ENABLE "Build the unittest code" OFF)

file(GLOB UTILITY_SRC "*.cc")
file(GLOB UNITTEST_SRC "*unittest.cc*")
list(REMOVE_ITEM UTILITY_SRC ${UNITTEST_SRC})

#include_directories(../../3rdparty/json/include)
include_directories(../)
add_library(utility ${UTILITY_SRC})
target_link_libraries(utility ssl crypto)
if(UNITTEST_ENABLE)
  foreach(sourcefile ${UNITTEST_SRC})
    string(REPLACE ".cc" "" appname ${sourcefile})
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" appname ${appname})
    add_executable(${appname} ${sourcefile})
    target_link_libraries(${appname} system_manager gtest pthread)
  endforeach(sourcefile ${UNITTEST_SRC})
endif(UNITTEST_ENABLE)
