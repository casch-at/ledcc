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
# Include current source dir and binary dir
# ---------------------------------------------------------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# ---------------------------------------------------------------------------------------------------
# Include directories containing headers files
# ---------------------------------------------------------------------------------------------------
set(include_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/core/animations
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/aqp
)
include_directories(${include_DIRECTORIES})
if(CMAKE_AUTOMOC)
    message(STATUS "Automoc on...")
endif()
if(CMAKE_INCLUDE_CURRENT_DIR)
    message(STATUS "Include current dir on...")
endif()
# ---------------------------------------------------------------------------------------------------
# Add subdirectory core where the Global.h.in file gets configured
# ---------------------------------------------------------------------------------------------------
add_subdirectory(core)

set(${PROJECT_NAME}_core_SOURCES
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
)

set(${PROJECT_NAME}_thirdparty_SOURCES
    ${ThirdPartyDirectory}/aqp/alt_key.cpp
    ${ThirdPartyDirectory}/aqp/aqp.cpp
    ${ThirdPartyDirectory}/aqp/kuhn_munkres.cpp
)

set(${PROJECT_NAME}_gui_SOURCES
    ${GuiDirectory}/MainWindow.cpp
    ${GuiDirectory}/SettingsDialog.cpp
    ${GuiDirectory}/AnimationListWidget.cpp
    ${GuiDirectory}/AnimationPlayListWidget.cpp
    ${GuiDirectory}/AnimationOptions.cpp
    ${GuiDirectory}/PortMessageBox.cpp
    ${GuiDirectory}/AnimationItem.cpp
    ${GuiDirectory}/AnimationPropertiesPreview.cpp
    ${GuiDirectory}/ListWidget.cpp
)

set(${PROJECT_NAME}_gui_FORMS
    ${GuiDirectory}/MainWindow.ui
    ${GuiDirectory}/SettingsDialog.ui
    ${GuiDirectory}/AnimationOptions.ui
    ${GuiDirectory}/AnimationPropertiesPreview.ui
)

set(${PROJECT_NAME}_MOC
    ${GuiDirectory}/AddDialog.hpp
    ${GuiDirectory}/AnimationItem.hpp
    ${GuiDirectory}/AnimationListWidget.hpp
    ${GuiDirectory}/AnimationPlayListWidget.hpp
    ${GuiDirectory}/AnimationOptions.hpp
    ${GuiDirectory}/AnimationPropertiesPreview.hpp
    ${GuiDirectory}/ListWidget.hpp
    ${GuiDirectory}/MainWindow.hpp
    ${GuiDirectory}/PorMessageBox.hpp
    ${GuiDirectory}/SettingsDialog.hpp
)

set(${PROJECT_NAME}_SOURCES
    ${${PROJECT_NAME}_core_SOURCES}
    ${${PROJECT_NAME}_thirdparty_SOURCES}
    ${${PROJECT_NAME}_gui_SOURCES}
)

set(${PROJECT_NAME}_SOURCES_MAINEXE main.cpp)

# ---------------------------------------------------------------------------------------------------
# Create a source file out of the resource file
# ---------------------------------------------------------------------------------------------------
qt5_add_resources(${PROJECT_NAME}_RESOURCES_RCC
                  icons.qrc OPTIONS -no-compress
)
# ---------------------------------------------------------------------------------------------------
# Create ui_xxxx.h files out of .ui files
# ---------------------------------------------------------------------------------------------------
qt5_wrap_ui(${PROJECT_NAME}_FORMS_HEADERS
            ${${PROJECT_NAME}_gui_FORMS}
)

# ---------------------------------------------------------------------------------------------------
# Create metaobject sourcecode files
# ---------------------------------------------------------------------------------------------------
qt5_wrap_cpp(${PROJECT_NAME}_MOC_SOURCES
             ${${PROJECT_NAME}_MOC}
)

# ---------------------------------------------------------------------------------------------------
# Create a static library of all sources
# ---------------------------------------------------------------------------------------------------
add_library(${PROJECT_NAME}_core
            STATIC
            ${${PROJECT_NAME}_SOURCES}
            ${${PROJECT_NAME}_FORMS_HEADERS}
            ${${PROJECT_NAME}_MOC_SOURCES}
)

# ---------------------------------------------------------------------------------------------------
# Create a executable
# ---------------------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME}
               WIN32 ${${PROJECT_NAME}_SOURCES_MAINEXE}
)

# ---------------------------------------------------------------------------------------------------
# Link libraries against the program
# ---------------------------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
                      ${${PROJECT_NAME}_core}
                      ${Qt5Core_LIBRARIES}
                      ${Qt5Gui_LIBRARIES}
                      ${Qt5SerialPort_LIBRARIES}
)

#target_link_libraries(${PROJECT_NAME}
#                      ${${PROJECT_NAME}_core}
#                      Qt5::Core
#                      Qt5::Widgets
#                      Qt5::Gui
#                      Qt5::SerialPort
#)
