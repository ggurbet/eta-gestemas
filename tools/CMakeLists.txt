set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)
find_package(X11)

set(xlibwmatool_SOURCES
  xlibwmatool.cpp
)

# xlibwmatool
add_executable(xlibwmatool xlibwmatool.cpp)
add_dependencies(xlibwmatool gestemas)

target_link_libraries(xlibwmatool
    Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB} ${FRAME_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(xlibwmatool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter)

# longpresstool
add_executable(longpresstool longpresstool.cpp)
add_dependencies(longpresstool gestemas)

target_link_libraries(longpresstool
  Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB}
  ${X11_XTest_LIB} ${FRAME_LIB}
  ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(longpresstool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/longpress.xml
    ${CMAKE_CURRENT_BINARY_DIR}/longpress.xml COPYONLY)

# twotouchpinchtool
add_executable(twotouchpinchtool twotouchpinchtool.cpp)
add_dependencies(twotouchpinchtool gestemas)

target_link_libraries(twotouchpinchtool
  Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB}
  ${X11_XTest_LIB} ${FRAME_LIB}
  ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(twotouchpinchtool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/twotouchpinch.xml
    ${CMAKE_CURRENT_BINARY_DIR}/twotouchpinch.xml COPYONLY)

# taptool
add_executable(taptool taptool.cpp)
add_dependencies(taptool gestemas)

target_link_libraries(taptool
  Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB}
  ${X11_XTest_LIB} ${FRAME_LIB}
  ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(taptool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tap.xml
    ${CMAKE_CURRENT_BINARY_DIR}/tap.xml COPYONLY)

# pantool
add_executable(pantool pantool.cpp)
add_dependencies(pantool gestemas)

target_link_libraries(pantool
  Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB}
  ${X11_XTest_LIB} ${FRAME_LIB}
  ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(pantool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pan.xml
    ${CMAKE_CURRENT_BINARY_DIR}/pan.xml COPYONLY)

# integrationtool
add_executable(integrationtool integrationtool.cpp)
add_dependencies(integrationtool gestemas)

target_link_libraries(integrationtool
  Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB}
  ${X11_XTest_LIB} ${FRAME_LIB}
  ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(integrationtool PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/integration.xml
    ${CMAKE_CURRENT_BINARY_DIR}/integration.xml COPYONLY)
