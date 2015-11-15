set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)
find_package(X11)

set(xlibwmatool_SOURCES
  xlibwmatool.cpp
)

add_executable(xlibwmatool ${xlibwmatool_SOURCES})

target_link_libraries(xlibwmatool
    Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(xlibwmatool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter)
