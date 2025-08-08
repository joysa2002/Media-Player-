cmake_minimum_required(VERSION 3.16)
project(SmartMediaPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Remove debug flags for release builds to avoid D3D11 debug layer issues
option(ENABLE_DEBUG_OUTPUT "Enable Qt debug output" OFF)  # Changed to OFF

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia MultimediaWidgets)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SmartMediaPlayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(SmartMediaPlayer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(SmartMediaPlayer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SmartMediaPlayer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Network
)

# Platform-specific multimedia backend preferences
if(WIN32)
    target_compile_definitions(SmartMediaPlayer PRIVATE
        -DQT_MULTIMEDIA_PREFERRED_PLUGINS=directshow,ffmpeg
        # Disable D3D11 debug layer for release builds
        $<$<CONFIG:Release>:-DQT_NO_DEBUG>
    )

    # For Windows, try to use software rendering if hardware fails
    target_compile_definitions(SmartMediaPlayer PRIVATE
        -DQT_OPENGL_ES_2_ANGLE_STATIC
    )

elseif(APPLE)
    target_compile_definitions(SmartMediaPlayer PRIVATE
        -DQT_MULTIMEDIA_PREFERRED_PLUGINS=avfoundation,ffmpeg
    )
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SmartMediaPlayer)
endif()

set_target_properties(SmartMediaPlayer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SmartMediaPlayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SmartMediaPlayer)
endif()

# Only enable debug output in debug builds
if(ENABLE_DEBUG_OUTPUT AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(SmartMediaPlayer PRIVATE -DQT_DEBUG)
endif()
