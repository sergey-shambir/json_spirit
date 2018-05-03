find_package(Boost 1.66 REQUIRED)

add_subdirectory(unittests)

if ("$ENV{CI_NAME}" NOT STREQUAL "TRAVIS")
add_subdirectory(integrationtests)
endif()
