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

# ---------------------------------------------------------------------------------------------------
# Copy qml files to binary directory share/qml
# ---------------------------------------------------------------------------------------------------
set(GLOB QML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml/*.qml)
foreach(qml_file ${QML_FILES})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml/${qml_file} ${CMAKE_CURRENT_BINARY_DIR}/share/qml/${qml_file}
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/preview/qml/${qml_file})
endforeach()

# ---------------------------------------------------------------------------------------------------
# Include directories containing headers files
# ---------------------------------------------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/core/animations
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/aqp
    ${CMAKE_CURRENT_SOURCE_DIR}/preview

    ${USB1_INCLUDE_DIRS}
)

# ---------------------------------------------------------------------------------------------------
# Configure header file to pass some data from cmake to the program
# ---------------------------------------------------------------------------------------------------
include(configure.h.in.cmake)

# ---------------------------------------------------------------------------------------------------
# Collect all source files
# ---------------------------------------------------------------------------------------------------
set(ledcc_core_SOURCES
    ${CORE_DIR}/Sender.cpp
    ${CORE_DIR}/animations/Draw.cpp
    ${CORE_DIR}/animations/Animation.cpp
    ${CORE_DIR}/animations/Lift.cpp
    ${CORE_DIR}/animations/Rain.cpp
    ${CORE_DIR}/animations/Wall.cpp
    ${CORE_DIR}/animations/RandomZLift.cpp
    ${CORE_DIR}/animations/Firework.cpp
    ${CORE_DIR}/animations/WireBoxCornerShrinkGrow.cpp
    ${CORE_DIR}/animations/WireBoxCenterShrinkGrow.cpp
    ${CORE_DIR}/animations/AxisNailWall.cpp
    ${CORE_DIR}/animations/Loadbar.cpp
    ${CORE_DIR}/animations/RandomSparkFlash.cpp
    ${CORE_DIR}/animations/RandomSpark.cpp
    ${CORE_DIR}/animations/RandomFiller.cpp
    ${CORE_DIR}/animations/StringFly.cpp
    ${CORE_DIR}/animations/AnimationHandler.cpp
    ${CORE_DIR}/animations/Animations.cpp
    ${CORE_DIR}/animations/ConwaysGameOfLife.cpp
    ${CORE_DIR}/animations/Wave.cpp
    ${CORE_DIR}/Global.hpp
    ${CORE_DIR}/Config.cpp
    ${CORE_DIR}/XmlPlaylistReader.cpp
    ${CORE_DIR}/XmlPlaylistWriter.cpp
    ${CORE_DIR}/System.cpp
    ${CORE_DIR}/animations/LookUpTables.hpp
    ${CORE_DIR}/USBInterface.cpp
)

set(ledcc_thirdparty_SOURCES
    ${THIRD_PARTY_DIR}/aqp/alt_key.cpp
    ${THIRD_PARTY_DIR}/aqp/aqp.cpp
    ${THIRD_PARTY_DIR}/aqp/kuhn_munkres.cpp
)

set(ledcc_gui_SOURCES
    ${GUI_DIR}/MainWindow.cpp
    ${GUI_DIR}/SettingsDialog.cpp
    ${GUI_DIR}/AnimationListWidget.cpp
    ${GUI_DIR}/AnimationPlaylistWidget.cpp
    ${GUI_DIR}/AnimationOptions.cpp
    ${GUI_DIR}/PortMessageBox.cpp
    ${GUI_DIR}/AnimationItem.cpp
    ${GUI_DIR}/AnimationPropertiesPreview.cpp
    ${GUI_DIR}/ListWidget.cpp
    ${GUI_DIR}/HelpDialog.cpp
    ${GUI_DIR}/AboutDialog.cpp
)
# ---------------------------------------------------------------------------------------------------
# Put all source files together
# ---------------------------------------------------------------------------------------------------
set(ledcc_SOURCES
    ${ledcc_core_SOURCES}
    ${ledcc_thirdparty_SOURCES}
    ${ledcc_gui_SOURCES}
)

# ---------------------------------------------------------------------------------------------------
# All ui forms
# ---------------------------------------------------------------------------------------------------
set(ledcc_gui_FORMS
    ${GUI_DIR}/MainWindow.ui
    ${GUI_DIR}/SettingsDialog.ui
    ${GUI_DIR}/AnimationOptions.ui
    ${GUI_DIR}/AnimationPropertiesPreview.ui
    ${GUI_DIR}/HelpDialog.ui
    ${GUI_DIR}/AboutDialog.ui
)

# ---------------------------------------------------------------------------------------------------
# Set translation files
# ---------------------------------------------------------------------------------------------------
set(ledcc_TS_FILES
    ${LANG_DIR}/${PROGNAME}_en.ts
    ${LANG_DIR}/${PROGNAME}_de.ts
)

# ---------------------------------------------------------------------------------------------------
# Set all libraries
# ---------------------------------------------------------------------------------------------------
set( ledcc_LIBRARIES
     ${Qt5Widgets_LIBRARIES}
     ${Qt5SerialPort_LIBRARIES}
#    ${Qt5WebKit_LIBRARIES}
     ${Qt5WebKitWidgets_LIBRARIES}

     ${USB1_LIBRARIES}
)

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
# Create translation files ts/qm
# NOTE:: Set UPDATE_TRANSLATIONS to ON when you want update the translation files. After that you
# shoulde set it to OFF as soon as possible, because when it set to ON and you call 'make clean'
# the translation files will also be deleted
# ---------------------------------------------------------------------------------------------------
set(_TMP ${CORE_DIR} ${GUI_DIR} )
if(UPDATE_TRANSLATIONS)
    message(STATUS "TS files will be created")
    qt5_create_translation(QM_FILES ${_TMP} ${ledcc_TS_FILES})
else(UPDATE_TRANSLATIONS)
    message(STATUS "QM files will be created")
    qt5_add_translation(QM_FILES ${ledcc_TS_FILES})
endif(UPDATE_TRANSLATIONS)
unset(_TMP)

# ---------------------------------------------------------------------------------------------------
# Create a static library of all sources.
# ---------------------------------------------------------------------------------------------------
add_library( ledcc-core
             STATIC
             ${ledcc_SOURCES}
             ${ledcc_FORMS_HEADERS}
             ${QM_FILES}
)

set_target_properties(ledcc-core PROPERTIES COMPILE_DEFINITIONS LED_CONTROL_BUILDING_CORE)

# ---------------------------------------------------------------------------------------------------
# Link libraries against the created library.
# HINT: Qt5Widgets_LIBRARIES includes Qt5Core_LIBRARIES and Qt5Gui_LIBRARIES
# ---------------------------------------------------------------------------------------------------
target_link_libraries( ledcc-core
                       ${ledcc_LIBRARIES}
)

# ---------------------------------------------------------------------------------------------------
# Create a executable
# ---------------------------------------------------------------------------------------------------
add_executable( ${PROGNAME}
                ${ledcc_SOURCES_MAINEXE}
                ${ledcc_RESOURCES_RCC}
)

#set_target_properties(${PROGNAME} PROPERTIES ENABLE_EXPORTS ON)

# ---------------------------------------------------------------------------------------------------
# The executable depends on the previously created static library ledcc_core.
# To insure that this library is first created add a dependency.
# ---------------------------------------------------------------------------------------------------
add_dependencies(${PROGNAME} ledcc-core ${ledcc_TS_FILES})

# ---------------------------------------------------------------------------------------------------
# Link libraries against the program.
# HINT: Qt5Widgets_LIBRARIES includes Qt5Core_LIBRARIES and Qt5Gui_LIBRARIES
# ---------------------------------------------------------------------------------------------------
target_link_libraries(${PROGNAME}
                      ledcc-core
)



#set_target_properties(${PROGNAME} ledcc-core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set_target_properties(${PROGNAME} ledcc-core PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set_target_properties(${PROGNAME} ledcc-core PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ---------------------------------------------------------------------------------------------------
# Create install target
# ---------------------------------------------------------------------------------------------------
install(TARGETS ${PROGNAME}
        BUNDLE DESTINATION . COMPONENT Runtime
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR} COMPONENT Runtime
)

#set_property(TARGET ${PROGNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(FILES ${QM_FILES} DESTINATION
        ${CMAKE_INSTALL_PREFIX}/${DATA_INSTALL_DIR}/ts
)

