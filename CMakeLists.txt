cmake_minimum_required(VERSION 3.5)

project(ppm2 LANGUAGES CXX)

SET(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Core REQUIRED)

#FIND_PACKAGE(Qt5Network)
qt5_add_resources(resources.qrc)
if(ANDROID)
  add_library(ppm2 SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    sockettest.cpp
    sockettest.h
    csv.h
    tcpthread.h
    tcpthread.cpp
  )
else()
  add_executable(ppm2
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    sockettest.cpp
    sockettest.h
    resources.qrc
    csv.h
    tcpthread.h
    tcpthread.cpp
  )
endif()

target_link_libraries(ppm2 PRIVATE Qt5::Widgets)
target_link_libraries(ppm2 PRIVATE Qt5::Network)
target_link_libraries(ppm2 PRIVATE Qt5::Core)
