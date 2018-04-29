cmake_minimum_required(VERSION 3.8)

project(json_spirit)

# True if you want to develop json_spirit library itself.
option(JSON_SPIRIT_CHECK "build tests and run static analyzer checks" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/functions.cmake)

if ("$ENV{CI_NAME}" STREQUAL "TRAVIS")
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup()
endif()

if(JSON_SPIRIT_CHECK)
# Enable CTest integration.
enable_testing()
# Enable Clang Tidy static analysys.
custom_clang_tidy_checks(
    "cppcoreguidelines-*"
    "readability-*"
    "modernize-*"
    "misc-*"
    "bugprone-*"
    "-clang-analyzer-*"
    "-clang-diagnostic-*"
    "-modernize-raw-string-literal"
    "-cppcoreguidelines-pro-type-member-init"
    "-cppcoreguidelines-pro-type-vararg"
    "-cppcoreguidelines-pro-type-union-access"
    "-cppcoreguidelines-pro-bounds-array-to-pointer-decay"
    "-cppcoreguidelines-pro-type-reinterpret-cast"
    "-cppcoreguidelines-pro-bounds-pointer-arithmetic"
)
endif()

subdirs(
  src
  tests
  samples
)

install(
  FILES
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_error_position.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_reader.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_reader_template.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_stream_reader.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_utils.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_value.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_writer.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_writer_template.h
  ${CMAKE_SOURCE_DIR}/json_spirit/json_spirit_writer_options.h
  DESTINATION include)

install(
  FILES
  ${CMAKE_BINARY_DIR}/json_spirit/libjson_spirit.a
  DESTINATION lib)

include(CPack)
