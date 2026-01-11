cmake_minimum_required(VERSION 3.16)
project(MusicPlayer LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    Library.h
    Library.cpp
    MainWindow.ui
    SongListsDialog.h
    SongListsDialog.cpp
    SongListsDialog.ui
    PlayerWindow.cpp
    PlayerWindow.h
    PlayerWindow.ui
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
# qt_add_executable(...) 바로 아래에 추가
get_target_property(_tgt_srcs ${PROJECT_NAME} SOURCES)
message(STATUS "TARGET SOURCES = ${_tgt_srcs}")
message(STATUS "PROJECT_SOURCES = ${PROJECT_SOURCES}")

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
)

if (MSVC)
    add_compile_options(/utf-8)
endif()

qt6_add_resources(APP_ICONS
    ${CMAKE_CURRENT_SOURCE_DIR}/icons.qrc
)

target_sources(${PROJECT_NAME} PRIVATE
    ${APP_ICONS}
)

message(STATUS "APP_ICONS = ${APP_ICONS}")

