cmake_minimum_required(VERSION 3.1)
project(main)

add_subdirectory(rest)
add_subdirectory(network)
add_subdirectory(system)
add_subdirectory(utility)
add_subdirectory(hal)

include_directories(./)

#file(GLOB APP_SOURCES "*.cc")
#add_library(rest ${APP_SOURCES})
#target_link_libraries(rest libpistache.a)

if(UNITTEST_ENABLE)
  add_definitions(-DDEVEL)
endif(UNITTEST_ENABLE)                                        

set(CMAKE_EXE_LINKER_FLAGS "")
add_executable(main main.cc)

if(UNITTEST_ENABLE)
target_link_libraries(main rest system_manager network_manager hal utility ssl crypto)
else()
target_link_libraries(main rest system_manager network_manager hal utility ssl crypto mmal_core mmal_util mmal_vc_client vcos bcm_host)

endif(UNITTEST_ENABLE)

install(TARGETS main
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
)
