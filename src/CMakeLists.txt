###############################################################################
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
################################################################################

#  !!!!!!!!!!!!!   Run Cmake with !!!!!!!!!!!!!!!!!!!!!!!
#   -DCMAKE_PREFIX_PATH=/opts/Qt5.2/5.2.0/gcc_64/lib/cmake/ -DCMAKE_BUILD_TYPE=DEBUG

# Widgets finds its own dependencies (QtGui and QtCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

include_directories (gui core aqp)

# Set Path variables
set(gui_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gui")
set(core_DIR "${CMAKE_CURRENT_SOURCE_DIR}/core")
set(aqp_DIR "${CMAKE_CURRENT_SOURCE_DIR}/aqp")
#set(cfiles_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cfiles/")


# configure a header file to pass some of the CMake settings
# to the source code
# # Configure "global.h"
# configure_file (
#   "${core_DIR}global.h.in"
#   "${core_DIR}global.h"
#   )

#message(STATUS ${${PROGNAME}_SOURCES_MAINEXE})
 # Get all files from the above set directories

set(MAINEXE     ../main.cpp)
set(RESOURCES   icons.qrc)
set(HEADERS     ${core_DIR}/Global.hpp)
set(GUI         ${gui_DIR}/DebugDockWidget.cpp
                ${gui_DIR}/MainWindow.cpp
                ${gui_DIR}/SettingsDialog.cpp
                ${gui_DIR}/AnimationListWidget.cpp
                ${gui_DIR}/AnimationPlayListWidget.cpp
                ${gui_DIR}/AnimationOptionsGroupBox.cpp
)
set(CORE        ${core_DIR}/animations/Draw.cpp
                ${core_DIR}/animations/Animation.cpp
                ${core_DIR}/animations/Lift.cpp
                ${core_DIR}/animations/Rain.cpp
                ${core_DIR}/animations/Wall.cpp
                ${core_DIR}/animations/RandomZLift.cpp
                ${core_DIR}/animations/Firework.cpp
                ${core_DIR}/animations/WireBoxCornerShrinkGrow.cpp
                ${core_DIR}/animations/WireBoxCenterShrinkGrow.cpp
                ${core_DIR}/animations/AxisNailWall.cpp
                ${core_DIR}/animations/Loadbar.cpp
                ${core_DIR}/animations/RandomSparkFlash.cpp
                ${core_DIR}/animations/RandomSpark.cpp
                ${core_DIR}/animations/RandomFiller.cpp
                ${core_DIR}/animations/StringFly.cpp
                ${core_DIR}/SendThread.cpp
)
set(AQP         ${aqp_DIR}/alt_key.cpp
                ${aqp_DIR}/aqp.cpp
                ${aqp_DIR}/kuhn_munkres.cpp
)
set(FORMS       ${gui_DIR}/DebugDockWidget.ui
                ${gui_DIR}/MainWindow.ui
                ${gui_DIR}/SettingsDialog.ui
                ${gui_DIR}/AnimationOptionsGroupBox.ui
)

#set(CFILES      ${cfiles}/AsciiHex.cpp


# PUT ALL SOURCE FILES TOGETHER
set(${PROGNAME}_SOURCES  ${GUI} ${CORE} ${AQP})

qt5_add_resources(${PROGNAME}_RESOURCES_RCC ${RESOURCES})
qt5_wrap_ui(${PROGNAME}_FORMS_HEADER        ${FORMS})
qt5_wrap_cpp(${PROGNAME}_HEADERS_MOC        ${HEADERS})

add_executable(${PROGNAME}  ${${PROGNAME}_SOURCES}
                            ${HEADERS}
                            ${MAINEXE}
                            ${${PROGNAME}_FORMS_HEADER}
                            ${${PROGNAME}_RESOURCES_RCC}
)

# The Qt5Widgets_LIBARIES variable also includes QtGui and QtCore
qt5_use_modules(${PROGNAME} Widgets 
                            SerialPort
)
                            
# Link all libraries against the program
target_link_libraries(${PROGNAME} ${Qt5Widgets_LIBRARIES}  )
