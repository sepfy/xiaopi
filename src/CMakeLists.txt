cmake_minimum_required(VERSION 3.1)
project(main)
add_subdirectory(rest)
add_subdirectory(network)
add_subdirectory(system)
add_subdirectory(utility)

include_directories(../3rdparty/json/include)
include_directories(../3rdparty/pistache/include)
include_directories(./)
link_directories(../3rdparty/pistache/build/src)

#file(GLOB APP_SOURCES "*.cc")
#add_library(rest ${APP_SOURCES})
#target_link_libraries(rest libpistache.a)

if(UNITTEST_ENABLE)
  add_definitions(-DDEVEL)
endif(UNITTEST_ENABLE)                                        

add_executable(main main.cc)
target_link_libraries(main rest system_manager network_manager utility ssl crypto)

install(TARGETS main
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
)
