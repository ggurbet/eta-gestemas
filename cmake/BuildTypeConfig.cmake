# Guard against bad build-type strings
if(NOT CMAKE_BUILD_TYPE)
  # Default is Debug
  set(CMAKE_BUILD_TYPE "Debug")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
string(TOUPPER "${CMAKE_BUILD_TYPE}" cmake_build_type_toupper)
if(    NOT cmake_build_type_tolower STREQUAL "debug"
    AND NOT cmake_build_type_tolower STREQUAL "release"
    AND NOT cmake_build_type_tolower STREQUAL "profile"
    AND NOT cmake_build_type_tolower STREQUAL "relwithdebinfo")
  message(FATAL_ERROR "Unknown build type \"${CMAKE_BUILD_TYPE}\". "
    "Allowed values are Debug, Release, Profile, RelWithDebInfo "
    "(case-insensitive).")
else ()
  message("Build type is ${CMAKE_BUILD_TYPE}")
endif ()
