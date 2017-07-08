cmake_minimum_required(VERSION 2.8.11)

project(untzy)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(QT_MIN_VERSION 5.6.0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra")
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
include_directories(${GSTREAMER_INCLUDE_DIRS})

add_subdirectory(lib/taglib)
include_directories(${CMAKE_BINARY_DIR}/lib/taglib/headers/taglib)

add_subdirectory(lib/googletest)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(tools/taglib)
