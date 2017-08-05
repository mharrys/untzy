qt5_add_resources(RESOURCES
    ../resources/resources.qrc
)

qt5_wrap_ui(UI
    ui/main_window.ui
    ui/engine_output.ui
)

set(SOURCES
    core/duration.cpp
    core/logger.cpp
    core/player.cpp
    core/volume.cpp
    core/song.cpp
    db/database.cpp
    db/playlist_row.cpp
    db/song_row.cpp
    engines/gstreamer_engine.cpp
    engines/gstreamer_pipeline.cpp
    engines/gstreamer_thread.cpp
    playlist/playlist_model.cpp
    ui/main_window.cpp
    ui/engine_output.cpp
    ui/widgets/playlist.cpp
    ui/widgets/playlist_tab.cpp
    ui/widgets/volume_slider.cpp
    ui/widgets/seeker.cpp
)

set(HEADERS
    core/duration.h
    core/logger.h
    core/player.h
    core/volume.h
    core/song.h
    db/database.h
    db/playlist_row.h
    db/song_row.h
    engines/engine.h
    engines/gstreamer_engine.h
    engines/gstreamer_pipeline.h
    engines/gstreamer_thread.h
    playlist/playlist_model.h
    ui/main_window.h
    ui/engine_output.h
    ui/widgets/playlist.h
    ui/widgets/playlist_tab.h
    ui/widgets/volume_slider.h
    ui/widgets/seeker.h
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
    Qt5::Sql
    tag
)

target_link_libraries(${PROJECT_NAME}
    untzy_lib
)
