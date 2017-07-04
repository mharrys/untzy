if (NOT BUILD_SHARED_LIBS)
  add_definitions(-DTAGLIB_STATIC)
endif()

add_executable(tagreader tagreader.cpp)
target_link_libraries(tagreader tag)

add_executable(tagwriter tagwriter.cpp)
target_link_libraries(tagwriter tag)

add_executable(framelist framelist.cpp)
target_link_libraries(framelist tag)

add_executable(strip-id3v1 strip-id3v1.cpp)
target_link_libraries(strip-id3v1 tag)
