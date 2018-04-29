
add_executable(json_spirit_integration_tests "main.cpp" "parsertests.cpp")
target_link_libraries(json_spirit_integration_tests PRIVATE json_spirit boost_system boost_filesystem dl)


target_link_libraries(json_spirit_integration_tests PRIVATE "-fsanitize=address")
target_compile_options(json_spirit_integration_tests PRIVATE "-pthread" "-fsanitize=address")

add_test(json_spirit_integration_tests json_spirit_integration_tests)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/integration_tests_data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

custom_enable_cxx14(json_spirit_integration_tests)
custom_enable_warnings(json_spirit_integration_tests)
