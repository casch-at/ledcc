# ---------------------------------------------------------------------------------------------------
#  Sub CMakeFile
#  Copyright (C) 2013 Christian Schwarzgruber <christiandev9@gmail.com>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 or (at your option)
#  version 3 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
# ---------------------------------------------------------------------------------------------------

#macro( marble_add_project_resources resources )
#  add_custom_target( ${PROJECT_NAME}_Resources ALL SOURCES ${ARGN} )
#endmacro()
#FILE (GLOB QML_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml/*.qml )
#marble_add_project_resources(${QML_FILES})

# ---------------------------------------------------------------------------------------------------
# Copy qml files to binary directory share/qml
# ---------------------------------------------------------------------------------------------------
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml DESTINATION ${CMAKE_BINARY_DIR}/share)
set(GLOB QML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml/*.qml)
foreach(qml_file ${QML_FILES})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml/${qml_file} ${CMAKE_CURRENT_BINARY_DIR}/share/qml/${qml_file}
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml/${qml_file})
endforeach()

# ---------------------------------------------------------------------------------------------------
# Set include directories containing headers files
# ---------------------------------------------------------------------------------------------------
set(include_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/core/animations
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/aqp
    ${CMAKE_CURRENT_SOURCE_DIR}/preview
)

include_directories(${include_DIRECTORIES})

# ---------------------------------------------------------------------------------------------------
# Configure header file to pass some data from cmake to the program
# ---------------------------------------------------------------------------------------------------
include(configure.h.in.cmake)

# ---------------------------------------------------------------------------------------------------
# Collect all source files
# ---------------------------------------------------------------------------------------------------
set(ledcc_core_SOURCES
    ${CoreDirectory}/Options.cpp
    ${CoreDirectory}/Sender.cpp
    ${CoreDirectory}/animations/Draw.cpp
    ${CoreDirectory}/animations/Animation.cpp
    ${CoreDirectory}/animations/Lift.cpp
    ${CoreDirectory}/animations/Rain.cpp
    ${CoreDirectory}/animations/Wall.cpp
    ${CoreDirectory}/animations/RandomZLift.cpp
    ${CoreDirectory}/animations/Firework.cpp
    ${CoreDirectory}/animations/WireBoxCornerShrinkGrow.cpp
    ${CoreDirectory}/animations/WireBoxCenterShrinkGrow.cpp
    ${CoreDirectory}/animations/AxisNailWall.cpp
    ${CoreDirectory}/animations/Loadbar.cpp
    ${CoreDirectory}/animations/RandomSparkFlash.cpp
    ${CoreDirectory}/animations/RandomSpark.cpp
    ${CoreDirectory}/animations/RandomFiller.cpp
    ${CoreDirectory}/animations/StringFly.cpp
    ${CoreDirectory}/animations/AnimationHandler.cpp
    ${CoreDirectory}/animations/Animations.cpp
    ${CoreDirectory}/Global.hpp
    ${CoreDirectory}/Config.cpp
    ${CoreDirectory}/XmlPlaylistReader.cpp
    ${CoreDirectory}/XmlPlaylistWriter.cpp
    ${CoreDirectory}/System.cpp
)

set(ledcc_thirdparty_SOURCES
    ${ThirdPartyDirectory}/aqp/alt_key.cpp
    ${ThirdPartyDirectory}/aqp/aqp.cpp
    ${ThirdPartyDirectory}/aqp/kuhn_munkres.cpp
)

set(ledcc_gui_SOURCES
    ${GuiDirectory}/MainWindow.cpp
    ${GuiDirectory}/SettingsDialog.cpp
    ${GuiDirectory}/AnimationListWidget.cpp
    ${GuiDirectory}/AnimationPlayListWidget.cpp
    ${GuiDirectory}/AnimationOptions.cpp
    ${GuiDirectory}/PortMessageBox.cpp
    ${GuiDirectory}/AnimationItem.cpp
    ${GuiDirectory}/AnimationPropertiesPreview.cpp
    ${GuiDirectory}/ListWidget.cpp
    ${GuiDirectory}/HelpDialog.cpp
)
# ---------------------------------------------------------------------------------------------------
# Put all source files together
# ---------------------------------------------------------------------------------------------------
set(ledcc_core_SOURCES
    ${ledcc_core_SOURCES}
    ${ledcc_thirdparty_SOURCES}
    ${ledcc_gui_SOURCES}
)

# ---------------------------------------------------------------------------------------------------
# All ui forms
# ---------------------------------------------------------------------------------------------------
set(ledcc_gui_FORMS
    ${GuiDirectory}/MainWindow.ui
    ${GuiDirectory}/SettingsDialog.ui
    ${GuiDirectory}/AnimationOptions.ui
    ${GuiDirectory}/AnimationPropertiesPreview.ui
    ${GuiDirectory}/HelpDialog.ui
)

# ---------------------------------------------------------------------------------------------------
# Headers files for what moc data has to be generated
# ---------------------------------------------------------------------------------------------------
#set(ledcc_MOC
#    ${GuiDirectory}/AddDialog.hpp
#    ${GuiDirectory}/AnimationItem.hpp
#    ${GuiDirectory}/AnimationListWidget.hpp
#    ${GuiDirectory}/AnimationPlayListWidget.hpp
#    ${GuiDirectory}/AnimationOptions.hpp
#    ${GuiDirectory}/AnimationPropertiesPreview.hpp
#    ${GuiDirectory}/ListWidget.hpp
#    ${GuiDirectory}/MainWindow.hpp
#    ${GuiDirectory}/PortMessageBox.hpp
#    ${GuiDirectory}/SettingsDialog.hpp
#)


# ---------------------------------------------------------------------------------------------------
# The main file
# ---------------------------------------------------------------------------------------------------
set(ledcc_SOURCES_MAINEXE
    main.cpp
)

# ---------------------------------------------------------------------------------------------------
# Create a source file out of the resource file.
# ---------------------------------------------------------------------------------------------------
qt5_add_resources(ledcc_RESOURCES_RCC
                  icons.qrc OPTIONS -no-compress
)
# ---------------------------------------------------------------------------------------------------
# Create ui_xxxx.h files out of .ui files.
# ---------------------------------------------------------------------------------------------------
qt5_wrap_ui(ledcc_FORMS_HEADERS
            ${ledcc_gui_FORMS}
)

# ---------------------------------------------------------------------------------------------------
# Create metaobject sourcecode files.
# ---------------------------------------------------------------------------------------------------
#qt5_wrap_cpp(ledcc_MOC_SOURCES
#             ${ledcc_MOC}
#)

# ---------------------------------------------------------------------------------------------------
# Create a static library of all sources.
# ---------------------------------------------------------------------------------------------------
add_library(ledcc-core
            STATIC
            ${ledcc_core_SOURCES}
            ${ledcc_FORMS_HEADERS}
)
# ---------------------------------------------------------------------------------------------------
# Link libraries against the created library.
# HINT: Qt5Widgets_LIBRARIES includes Qt5Core_LIBRARIES and Qt5Gui_LIBRARIES
# ---------------------------------------------------------------------------------------------------
target_link_libraries(ledcc-core
                      ${Qt5Widgets_LIBRARIES}
                      ${Qt5SerialPort_LIBRARIES}
#                      ${Qt5WebKit_LIBRARIES}
                      ${Qt5WebKitWidgets_LIBRARIES}
)
# ---------------------------------------------------------------------------------------------------
# Create a executable
# ---------------------------------------------------------------------------------------------------
add_executable(${PROGNAME}
               ${ledcc_SOURCES_MAINEXE}
               ${ledcc_RESOURCES_RCC}
)
# ---------------------------------------------------------------------------------------------------
# The executable depends on the previously created static library ledcc_core.
# To insure that this library is first created add a dependency.
# ---------------------------------------------------------------------------------------------------
add_dependencies(${PROGNAME} ledcc-core)

# ---------------------------------------------------------------------------------------------------
# Link libraries against the program.
# HINT: Qt5Widgets_LIBRARIES includes Qt5Core_LIBRARIES and Qt5Gui_LIBRARIES
# ---------------------------------------------------------------------------------------------------
target_link_libraries(${PROGNAME}
                      ledcc-core
)


set_target_properties(${PROGNAME} ledcc-core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set_target_properties(${PROGNAME} ledcc-core PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set_target_properties(${PROGNAME} ledcc-core PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
