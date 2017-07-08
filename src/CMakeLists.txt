qt5_add_resources(RESOURCES
    ../resources/resources.qrc
)

qt5_wrap_ui(UI
    ui/main_window.ui
    ui/engine_output.ui
)

set(SOURCES
    core/logger.cpp
    core/player.cpp
    core/volume.cpp
    engines/gstreamer_engine.cpp
    engines/gstreamer_pipeline.cpp
    engines/gstreamer_thread.cpp
    ui/main_window.cpp
    ui/engine_output.cpp
)

set(HEADERS
    core/logger.h
    core/player.h
    core/volume.h
    engines/engine.h
    engines/gstreamer_engine.h
    engines/gstreamer_pipeline.h
    engines/gstreamer_thread.h
    ui/main_window.h
    ui/engine_output.h
)

set(EXECUTABLE_OUTPUT_PATH ..)

# build lib for test to depend on
add_library(untzy_lib
    ${SOURCES}
    ${HEADERS}
    ${UI}
    ${RESOURCES}
)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(untzy_lib
    ${GSTREAMER_LIBRARIES}
    Qt5::Widgets
    tag
)

target_link_libraries(${PROJECT_NAME}
    untzy_lib
)
