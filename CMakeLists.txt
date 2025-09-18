cmake_minimum_required(VERSION 3.8)
project(botspeak)

option(TESTING "Build the test_real_world executable" OFF)

add_library(botspeak
  src/bot_speak_frame_ops.c
)

include(GNUInstallDirs)

target_include_directories(botspeak PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(botspeak_be
  src/bot_speak_frame_ops_be.c
)

target_include_directories(botspeak_be PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(GNUInstallDirs)

install(DIRECTORY
  include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install and export the usual library (little-endian)
install(TARGETS botspeak
  EXPORT botspeak_targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY
  include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT botspeak_targets
  FILE botspeakTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/botspeak
)

# Install and export the big-endian library
install(TARGETS botspeak_be
  EXPORT botspeak_be_targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT botspeak_be_targets
  FILE botspeak_beTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/botspeak
)


include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/botspeakConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/botspeak
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/botspeakConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/botspeak
)

# Usual (little-endian) test
add_executable(test_frame_ops
  test/test_frame_ops.c
)
target_link_libraries(test_frame_ops botspeak)

install(TARGETS
  test_frame_ops
  RUNTIME DESTINATION bin
)

# Big-endian test
add_executable(test_frame_ops_be
  test/test_frame_ops_be.c
)
target_link_libraries(test_frame_ops_be botspeak_be)

install(TARGETS
  test_frame_ops_be
  RUNTIME DESTINATION bin
)

# Optional test
if(TESTING)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(Serial REQUIRED IMPORTED_TARGET libserial)

  add_executable(test_real_world
    test/test_real_world.cpp
  )
  target_link_libraries(test_real_world botspeak PkgConfig::Serial)

  install(TARGETS
      test_real_world
      RUNTIME DESTINATION bin
  )
endif()