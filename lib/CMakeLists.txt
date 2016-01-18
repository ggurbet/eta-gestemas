set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core)
find_package(X11)
find_package(Frame)

set(libgestemas_SOURCES
  WindowManagerAdapter/windowmanageradapter.cpp
  WindowManagerAdapter/xlibwindowmanageradapter.cpp
  WindowManagerAdapter/windowmanageradapterlistener.cpp
  TargetFactory/target.cpp
  TargetFactory/targetfactory.cpp
  GestureRecognizer/state.cpp
  GestureRecognizer/gesturerecognizermanager.cpp
  GestureRecognizer/touch.cpp
  GestureRecognizer/gesturerecognizer.cpp
  GestureRecognizer/longpressgesturerecognizer.cpp
  GestureRecognizer/pangesturerecognizer.cpp
  GestureRecognizer/twotouchpinchgesturerecognizer.cpp
  GestureRecognizer/tapgesturerecognizer.cpp
  TouchManager/touchmanager.cpp
  TouchManager/libframetouchmanager.cpp
  GestureListener/gesturelistener.cpp
  GestureListener/xtestgesturelistener.cpp
  GestureListener/rightclick.cpp
  GestureListener/leftclick.cpp
  GestureListener/zoom.cpp
  GestureListener/drag.cpp
  GestureListener/scroll.cpp
  GestureListener/move.cpp
)

set(libgestemas_HEADERS
  utilities.h
  WindowManagerAdapter/windowmanageradapter.h
  WindowManagerAdapter/windowmanageradapterlistenerinterface.h
  WindowManagerAdapter/windowmanageradapterlistener.h
  WindowManagerAdapter/xlibwindowmanageradapter_p.h
  WindowManagerAdapter/xlibwindowmanageradapter.h
  TargetFactory/target.h
  TargetFactory/targetfactory.h
  GestureRecognizer/state.h
  GestureRecognizer/gesturerecognizermanager.h
  GestureRecognizer/touch.h
  GestureRecognizer/gesturerecognizer.h
  GestureRecognizer/longpressgesturerecognizer.h
  GestureRecognizer/pangesturerecognizer.h
  GestureRecognizer/twotouchpinchgesturerecognizer.h
  GestureRecognizer/tapgesturerecognizer.h
  TouchManager/touchmanager.h
  TouchManager/libframetouchmanager.h
  GestureListener/gesturelistener.h
  GestureListener/xtestgesturelistener.h
  GestureListener/rightclick.h
  GestureListener/leftclick.h
  GestureListener/zoom.h
  GestureListener/drag.h
  GestureListener/scroll.h
  GestureListener/move.h
)

add_library(gestemas STATIC ${libgestemas_SOURCES} ${libgesmetas_HEADERS})

target_link_libraries(gestemas Qt5::Core
  ${X11_X11_LIB} ${X11_Xi_LIB} ${X11_XTest_LIB} ${FRAME_LIB})

target_include_directories(gestemas PRIVATE
  ${CMAKE_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)
