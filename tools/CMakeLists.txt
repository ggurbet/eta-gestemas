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

# longpresstool
add_executable(longpresstool longpresstool.cpp)

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
