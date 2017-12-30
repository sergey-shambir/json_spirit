set(JSON_MAP_DEMO_SRCS json_map_demo.cpp)

add_executable(json_map_demo ${JSON_MAP_DEMO_SRCS})
target_link_libraries(json_map_demo json_spirit)
