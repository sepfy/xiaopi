cmake_minimum_required(VERSION 3.1)
project(main)

option(RASPIOS "Build with Raspberry Pi OS")
if(RASPIOS)
  add_definitions(-DRASPIOS)
  set(TENSORFLOW ${CMAKE_SOURCE_DIR}/third_party/tensorflow)
  set(LIVE555 ${CMAKE_SOURCE_DIR}/third_party/live555)
  include_directories("${TENSORFLOW}"
   "${TENSORFLOW}/cmake/abseil/absl"
   "${TENSORFLOW}/cmake/flatbuffers/include/"
   "${TENSORFLOW}/cmake/XNNPACK/include/"
   "${LIVE555}/liveMedia/include"
   "${LIVE555}/groupsock/include"
   "${LIVE555}/BasicUsageEnvironment/include"
   "${LIVE555}/UsageEnvironment/include"
   "/opt/vc/include"
  )

  link_directories("${TENSORFLOW}"
   "${TENSORFLOW}/cmake/"
   "${TENSORFLOW}/cmake/_deps/xnnpack-build/"
   "${TENSORFLOW}/cmake/pthreadpool/"
   "${TENSORFLOW}/cmake/cpuinfo/"
   "${TENSORFLOW}/cmake/clog/"
   "${TENSORFLOW}/cmake/_deps/ruy-build/"
   "${TENSORFLOW}/cmake/_deps/flatbuffers-build/"
   "${TENSORFLOW}/cmake/_deps/fft2d-build/"
   "${TENSORFLOW}/cmake/_deps/fft2d-build/"
   "${TENSORFLOW}/cmake/_deps/farmhash-build/"
   "${LIVE555}/liveMedia"
   "${LIVE555}/groupsock"
   "${LIVE555}/BasicUsageEnvironment"
   "${LIVE555}/UsageEnvironment"
   "/opt/vc/lib"
  )

endif()

add_subdirectory(system)
add_subdirectory(utility)
add_subdirectory(detection)
add_subdirectory(hal)
add_subdirectory(media)

if(NOT RASPIOS)
  add_subdirectory(network)
  add_subdirectory(rtc)
  add_subdirectory(rest)
  find_package(PkgConfig)
  pkg_check_modules(GLIB REQUIRED glib-2.0)
  include_directories(${GLIB_INCLUDE_DIRS})
endif()


if(UNITTEST_ENABLE)
  add_definitions(-DDEVEL)
endif(UNITTEST_ENABLE)

include_directories(./)
set(CMAKE_EXE_LINKER_FLAGS "")
add_executable(main main.cc)

if(UNITTEST_ENABLE)
target_link_libraries(main rest system_manager network_manager hal utility ssl crypto)
endif(UNITTEST_ENABLE)

target_link_libraries(main system_manager hal utility detection media ssl crypto mmal_core mmal_util mmal_vc_client vcos bcm_host opencv_core opencv_imgproc opencv_imgcodecs tensorflow-lite XNNPACK pthreadpool cpuinfo clog dl atomic pthread liveMedia groupsock BasicUsageEnvironment UsageEnvironment)

if(RASPIOS)
target_link_libraries(flatbuffers farmhash fft2d_fftsg ruy fft2d_fftsg2d)
else()
target_link_libraries(main rest network_manager rtc_agent pear nice srtp2 paho-mqtt3cs rtp curl)
endif()

install(TARGETS main
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
)
