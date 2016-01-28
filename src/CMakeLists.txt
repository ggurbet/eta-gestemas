set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core DBus)
find_package(X11)

set(gestemas_SOURCES
  main.cpp
)

add_executable(${GESTEMAS_APPLICATION_NAME} ${gestemas_SOURCES})
add_dependencies(${GESTEMAS_APPLICATION_NAME} gestemas)

target_link_libraries(${GESTEMAS_APPLICATION_NAME}
  Qt5::Core Qt5::DBus
  ${X11_X11_LIB} ${X11_Xi_LIB}
  ${X11_XTest_LIB} ${FRAME_LIB}
  ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(${GESTEMAS_APPLICATION_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)

include(ConfigureHeaderFile)
ConfigureHeaderFile("config.h")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/recognizers.xml
    ${CMAKE_CURRENT_BINARY_DIR}/recognizers.xml COPYONLY)

install(TARGETS ${GESTEMAS_APPLICATION_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install (FILES recognizers.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/${GESTEMAS_SYSTEM_PATH})
