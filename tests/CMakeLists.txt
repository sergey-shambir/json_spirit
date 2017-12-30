set(JSON_TEST_SRCS
    json_spirit_reader_test.cpp json_spirit_reader_test.h
    json_spirit_stream_reader_test.cpp json_spirit_stream_reader_test.h
    json_spirit_utils_test.cpp json_spirit_utils_test.h
    json_spirit_value_test.cpp json_spirit_value_test.h
    json_spirit_writer_test.cpp json_spirit_writer_test.h
    utils_test.cpp utils_test.h
    json_test.cpp
)

add_executable(json_test ${JSON_TEST_SRCS})
target_link_libraries(json_test json_spirit)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(json_test PRIVATE "-std=c++98")
endif()
