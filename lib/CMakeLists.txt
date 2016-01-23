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
  GestureRecognizer/swipegesturerecognizer.cpp
  TouchManager/touchmanager.cpp
  TouchManager/libframetouchmanager.cpp
  GestureListener/longpresslistener.cpp
  GestureListener/twotouchpinchlistener.cpp
  GestureListener/panlistener.cpp
  GestureListener/taplistener.cpp
  GestureListener/swipelistener.cpp
  GestureListener/xtest.cpp
  GestureListener/xtestshortcut.cpp
  GestureListener/xtestlongpressrightclick.cpp
  GestureListener/xtestlongpressmove.cpp
  GestureListener/xtesttwotouchpinchzoom.cpp
  GestureListener/xtestpanscroll.cpp
  GestureListener/xtestpanmove.cpp
  GestureListener/xtesttaprightclick.cpp
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
  GestureRecognizer/swipegesturerecognizer.h
  TouchManager/touchmanager.h
  TouchManager/libframetouchmanager.h
  GestureListener/gesturelistener.h
  GestureListener/longpresslistener.h
  GestureListener/twotouchpinchlistener.h
  GestureListener/panlistener.h
  GestureListener/taplistener.h
  GestureListener/swipelistener.h
  GestureListener/xtest.h
  GestureListener/xtestshortcut.h
  GestureListener/xtestlongpressrightclick.h
  GestureListener/xtestlongpressmove.h
  GestureListener/xtesttwotouchpinchzoom.h
  GestureListener/xtestpanscroll.h
  GestureListener/xtestpanmove.h
  GestureListener/xtesttaprightclick.h
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
