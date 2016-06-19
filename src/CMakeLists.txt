qt5_add_resources(RESOURCES
    ../resources/resources.qrc
)

qt5_wrap_ui(UI
    ui/main_window.ui
)

set(SOURCES
    core/logger.cpp
    core/player.cpp
    core/volume.cpp
    engines/gstreamer_engine.cpp
    engines/gstreamer_pipeline.cpp
    ui/main_window.cpp
)

set(HEADERS
    core/logger.h
    core/player.h
    core/volume.h
    engines/engine.h
    engines/gstreamer_engine.h
    engines/gstreamer_pipeline.h
    ui/main_window.h
)

set(EXECUTABLE_OUTPUT_PATH ..)

add_executable(${PROJECT_NAME}
    main.cpp
    ${SOURCES}
    ${HEADERS}
    ${UI}
    ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME}
    ${GSTREAMER_LIBRARIES}
    Qt5::Widgets
)
