qt5_add_resources(RESOURCES
    ../resources/resources.qrc
)

qt5_wrap_ui(UI
    ui/main_window.ui
    ui/engine_output.ui
    ui/playlist_widget.ui
)

set(SOURCES
    core/logger.cpp
    core/player.cpp
    core/volume.cpp
    core/duration.cpp
    core/song.cpp
    engines/gstreamer_engine.cpp
    engines/gstreamer_pipeline.cpp
    engines/gstreamer_thread.cpp
    playlist/playlist_model.cpp
    ui/main_window.cpp
    ui/engine_output.cpp
    ui/playlist_widget.cpp
)

set(HEADERS
    core/logger.h
    core/player.h
    core/volume.h
    core/duration.h
    core/song.h
    engines/engine.h
    engines/gstreamer_engine.h
    engines/gstreamer_pipeline.h
    engines/gstreamer_thread.h
    playlist/playlist_model.h
    ui/main_window.h
    ui/engine_output.h
    ui/playlist_widget.h
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
