cmake_minimum_required(VERSION 3.5)

project(Airceiver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

set(PROJECT_SOURCES
  main.cpp
  mainwindow.cpp
  mainwindow.h
  qtutils.cpp
  qtutils.h
  server.cpp
  server.h
)

add_executable(Airceiver
  ${PROJECT_SOURCES}
)

target_link_libraries(Airceiver PRIVATE
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(Airceiver PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER com.itsmattkc.Airceiver
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
