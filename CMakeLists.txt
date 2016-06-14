cmake_minimum_required(VERSION 2.8.11)

project(untzy)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(QT_MIN_VERSION 5.6.0)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(src)
