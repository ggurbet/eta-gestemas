set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)
find_package(X11)

set(libgestemas_SOURCES
  WindowManagerAdapter/windowmanageradapter.cpp
  WindowManagerAdapter/xlibwindowmanageradapter.cpp
)

set(libgestemas_HEADERS
  WindowManagerAdapter/windowmanageradapter.h
  WindowManagerAdapter/windowmanageradapter_tst.h
  WindowManagerAdapter/xlibwindowmanageradapter_p.h
  WindowManagerAdapter/xlibwindowmanageradapter.h
)

add_library(gestemas STATIC ${libgestemas_SOURCES} ${libgesmetas_HEADERS})

target_link_libraries(gestemas Qt5::Core ${X11_X11_LIB} ${X11_Xi_LIB})
