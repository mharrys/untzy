qt5_add_resources(RESOURCES
    ../resources/resources.qrc
)

qt5_wrap_ui(UI
    ui/main_window.ui
)

set(SOURCES
    core/player.cpp
    engines/gstreamer_engine.cpp
    engines/gstreamer_pipeline.cpp
    ui/main_window.cpp
)

set(HEADERS
    core/player.h
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
