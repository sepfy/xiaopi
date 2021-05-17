cmake_minimum_required(VERSION 3.1)
project(rtc_agent)

file(GLOB LIB_SRC "*.cc")
#file(GLOB UNITTEST_SRC "*test.cc*")
#list(REMOVE_ITEM LIB_SRC ${UNITTEST_SRC})

include_directories(../../3rdparty/json/include)
include_directories(../)
add_library(rtc_agent ${LIB_SRC})
target_link_libraries(rtc_agent utility pear glib-2.0 rtp)

