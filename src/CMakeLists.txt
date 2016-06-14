qt5_add_resources(
    RESOURCES
    ../resources/resources.qrc
)

qt5_wrap_ui(
    UI
    ui/mainwindow.ui
)

set(SOURCES
    ui/mainwindow.cpp
    ui/mainwindow.h
)

set(EXECUTABLE_OUTPUT_PATH ..)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${UI} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
