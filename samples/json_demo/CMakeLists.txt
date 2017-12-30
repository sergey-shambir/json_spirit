set(JSON_DEMO_SRCS json_demo.cpp)

add_executable(json_demo ${JSON_DEMO_SRCS})
target_link_libraries(json_demo json_spirit)

