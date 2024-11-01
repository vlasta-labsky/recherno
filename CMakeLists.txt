cmake_minimum_required(VERSION 3.12)

project(chernobyl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(PkgConfig REQUIRED)
pkg_check_modules(UNICORN REQUIRED unicorn)
include_directories(${UNICORN_INCLUDE_DIRS})

pkg_check_modules(CAPSTONE REQUIRED capstone)
include_directories(${CAPSTONE_INCLUDE_DIRS})

include_directories("include")

add_custom_target(nonbuilt ALL
    SOURCES TODO.md LICENSE README.md)
    
add_subdirectory(src)
