find_package(Boost 1.66 REQUIRED)

add_subdirectory(unittests)

if (NOT "$ENV{CI_NAME}" STREQUAL "TRAVIS")
add_subdirectory(integrationtests)
endif()
