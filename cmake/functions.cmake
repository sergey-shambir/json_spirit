
function(custom_enable_cxx17 TARGET)
    if (MSVC)
        set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "/std:c++latest")
    endif()
    target_compile_features(${TARGET} PUBLIC cxx_std_17)
endfunction()

function(custom_enable_cxx14 TARGET)
    target_compile_features(${TARGET} PUBLIC cxx_std_14)
endfunction()

function(custom_enable_cxx11 TARGET)
    target_compile_features(${TARGET} PUBLIC cxx_std_11)
endfunction()

function(custom_enable_warnings TARGET)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU)
        # TODO: maybe use "-Wduplicated-cond" "-Wduplicated-branches" "-Wlogical-op" "-Wnull-dereference" "-Wold-style-cast"
        target_compile_options(${TARGET} PRIVATE "-Wall" "-Wextra")
    endif()
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang)
        target_compile_options(${TARGET} PRIVATE "-Wall" "-Wextra" "-Wdocumentation")
    endif()
endfunction()
