#--------------------------------------------------------------------------------
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
#--------------------------------------------------------------------------------
#   Run Cmake with: -DCMAKE_PREFIX_PATH=/opts/Qt5.2/5.2.0/gcc_64/lib/cmake/ -DCMAKE_BUILD_TYPE=DEBUG
#--------------------------------------------------------------------------------
#   Directory -> src
#--------------------------------------------------------------------------------

# Set version numbers for global.h configuration. See CMakeLists.txt in /src/core directory


include(sourceconfig)

set(include_DIRECTORIES include_directories(gui core core/animations aqp


set(led_cube_core_SOURCES
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
)

set(led_cube_thirdparty_SOURCES
    ${ThirdPartyDirectory}/alt_key.cpp
    ${ThirdPartyDirectory}/aqp.cpp
    ${ThirdPartyDirectory}/kuhn_munkres.cpp
)

set(led_cube_gui_SOURCES
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

set(led_cube_gui_FORMS
    ${GuiDirectory}/MainWindow.ui
    ${GuiDirectory}/SettingsDialog.ui
    ${GuiDirectory}/AnimationOptions.ui
    ${GuiDirectory}/AnimationPropertiesPreview.ui
)

set(led_cube_main_SOURCE main.cpp)

qt5_wrap_ui(${PROGNAME}_FORMS_HEADER        ${FORMS})
qt5_wrap_cpp(${PROGNAME}_HEADERS_MOC        ${HEADERS})

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")


# Create resource file *.cpp
qt5_add_resources(${PROGNAME}_RESOURCES_RCC icons.qrc OPTIONS -no-compress )

add_executable(${PROGNAME} main.cpp ${${PROGNAME}_RESOURCES_RCC})
add_dependencies(${PROGNAME} )

# link against ledcube and animations
target_link_libraries(${PROGNAME} ledcubegui)


