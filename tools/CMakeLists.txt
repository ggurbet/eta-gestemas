set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)

set(tool1_SOURCES
  tool1.cpp
)

set(tool1_HEADERS
  # ../src/xxxx.h
)

add_executable(tool1 ${tool1_SOURCES} ${tool1_HEADERS})

target_link_libraries(tool1 Qt5::Core)

target_include_directories(tool1 PRIVATE ../src)
