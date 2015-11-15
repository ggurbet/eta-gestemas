set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)
find_package(X11)

set(gestemas_SOURCES
  main.cpp
)

add_executable(${GESTEMAS_APPLICATION_NAME} ${gestemas_SOURCES})

target_link_libraries(${GESTEMAS_APPLICATION_NAME}
    Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(${GESTEMAS_APPLICATION_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter)

include(ConfigureHeaderFile)
ConfigureHeaderFile("config.h")

install(TARGETS ${GESTEMAS_APPLICATION_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
