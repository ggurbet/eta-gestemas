cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(eta-gestemas)

# Enable c, and c++ language
enable_language(C CXX)

# Application name
set(GESTEMAS_APPLICATION_NAME "eta-gestemas")

# Application description
set(GESTEMAS_APPLICATION_DESCRIPTION
  "System-wide Multitouch Gesture Recognition Application.")

# Project version
set(GESTEMAS_VERSION_MAJOR 0)
set(GESTEMAS_VERSION_MINOR 1)
set(GESTEMAS_VERSION_PATCH 0)
set(GESTEMAS_VERSION
  "${GESTEMAS_VERSION_MAJOR}.${GESTEMAS_VERSION_MINOR}.${GESTEMAS_VERSION_PATCH}")

# Location of additional cmake modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(OutOfSourceConfig)
include(BuildTypeConfig)

option(BUILD_GESTEMAS "Build ultimate target gesmetas" ON)
option(BUILD_TOOLS "Build tests that require user interaction" ON)
option(BUILD_TEST "Build automated unit tests" ON)
option(BUILD_MAN "Build manual" OFF)
option(BUILD_DOC "Build documentation" OFF)

if(BUILD_GESTEMAS)
    add_subdirectory(src)
endif()

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

if(BUILD_TEST)
    add_subdirectory(test)
endif()

if(BUILD_MAN)
    add_subdirectory(man)
endif()

if(BUILD_DOC)
    add_subdirectory(doc)
endif()
