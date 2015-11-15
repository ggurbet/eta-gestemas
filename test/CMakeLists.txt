set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

enable_testing()

find_package(Qt5 COMPONENTS Core Widgets Test)
find_package(X11)

set(xlibwmatest_SOURCES
  xlibwmatest.cpp
)

add_executable(xlibwmatest ${xlibwmatest_SOURCES})
add_test(xlibwmatest xlibwmatest)

target_link_libraries(xlibwmatest
    Qt5::Core Qt5::Widgets Qt5::Test
    ${X11_X11_LIB} ${X11_Xi_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(xlibwmatest PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter)
