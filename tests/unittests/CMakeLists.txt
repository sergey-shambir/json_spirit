set(JSON_SPIRIT_UNIT_TESTS_SRCS
    json_spirit_reader_test.cpp
    json_spirit_stream_reader_test.cpp
    json_spirit_value_test.cpp
    json_spirit_writer_test.cpp
    stdafx.cpp stdafx.h
    test_utils.cpp test_utils.h
    main.cpp
)

add_executable(json_spirit_unit_tests ${JSON_SPIRIT_UNIT_TESTS_SRCS})
target_link_libraries(json_spirit_unit_tests json_spirit)
add_test(json_spirit_unit_tests json_spirit_unit_tests)

custom_enable_cxx14(json_spirit_unit_tests)
custom_enable_warnings(json_spirit_unit_tests)
