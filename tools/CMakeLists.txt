set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)
find_package(X11)

set(xlibwmatool_SOURCES
  xlibwmatool.cpp
)

# xlibwmatool
add_executable(xlibwmatool xlibwmatool.cpp)

target_link_libraries(xlibwmatool
    Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB} ${FRAME_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(xlibwmatool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter)

# touchmanagertool
add_executable(touchmanagertool touchmanagertool.cpp)
add_test(touchmanagertool touchmanagertool)

target_link_libraries(touchmanagertool
    Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB} ${FRAME_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(touchmanagertool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.xml
    ${CMAKE_CURRENT_BINARY_DIR}/config.xml COPYONLY)
