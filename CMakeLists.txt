cmake_minimum_required(VERSION 3.8)
project(botspeak)

option(TESTING "Build the test_real_world executable" OFF)

if(TESTING)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(Serial REQUIRED IMPORTED_TARGET libserial)
endif()

add_library(botspeak SHARED
  src/bot_speak_frame_ops.c
)

target_include_directories(botspeak PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(test_frame_ops
  test/test_frame_ops.c
)
target_link_libraries(test_frame_ops botspeak)

if(TESTING)
  add_executable(test_real_world
    test/test_real_world.cpp
  )
  target_link_libraries(test_real_world botspeak PkgConfig::Serial)
endif()

install(TARGETS
  botspeak
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(TARGETS
    test_frame_ops
    RUNTIME DESTINATION bin
)

if(TESTING)
  install(TARGETS
      test_real_world
      RUNTIME DESTINATION bin
  )
endif()

install(DIRECTORY
  include/
  DESTINATION include
)
