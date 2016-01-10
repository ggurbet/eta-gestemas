set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

enable_testing()

find_package(Qt5 COMPONENTS Core Widgets Test)
find_package(X11)

# xlibwmatest
add_executable(xlibwmatest xlibwmatest.cpp)
add_dependencies(xlibwmatest gestemas)
add_test(xlibwmatest xlibwmatest)

target_link_libraries(xlibwmatest
    Qt5::Core Qt5::Widgets Qt5::Test
    ${X11_X11_LIB} ${X11_Xi_LIB} ${FRAME_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(xlibwmatest PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter)

# targetfactorytest
add_executable(targetfactorytest targetfactorytest.cpp)
add_dependencies(targetfactorytest gestemas)
add_test(targetfactorytest targetfactorytest)

target_link_libraries(targetfactorytest
    Qt5::Core Qt5::Test
    ${X11_X11_LIB} ${X11_Xi_LIB} ${X11_XTest_LIB} ${FRAME_LIB}
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(targetfactorytest PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/WindowManagerAdapter
  ${CMAKE_SOURCE_DIR}/lib/TouchManager
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer
  ${CMAKE_SOURCE_DIR}/lib/TargetFactory
  ${CMAKE_SOURCE_DIR}/lib/GestureListener)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.xml
    ${CMAKE_CURRENT_BINARY_DIR}/config.xml COPYONLY)

# statetest
add_executable(statetest statetest.cpp)
add_dependencies(statetest gestemas)
add_test(statetest statetest)

target_link_libraries(statetest
    Qt5::Core Qt5::Test
    ${CMAKE_BINARY_DIR}/lib/libgestemas.a)

target_include_directories(statetest PRIVATE
  ${CMAKE_SOURCE_DIR}/lib/GestureRecognizer)
