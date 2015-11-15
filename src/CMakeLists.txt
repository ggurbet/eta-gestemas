set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)

set(gestemas_SOURCES
  main.cpp
)

set(gestemas_HEADERS
  # xxxx.h
)

add_executable(${GESTEMAS_APPLICATION_NAME} ${gestemas_SOURCES} ${gestemas_HEADERS})

target_link_libraries(${GESTEMAS_APPLICATION_NAME} Qt5::Core)

include(ConfigureHeaderFile)
ConfigureHeaderFile("config.h")

install(TARGETS ${GESTEMAS_APPLICATION_NAME} DESTINATION /bin)
