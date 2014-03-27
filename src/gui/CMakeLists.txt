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
#   Directory -> gui
#--------------------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
set(GUI  #DebugDockWidget.cpp
         MainWindow.cpp
         SettingsDialog.cpp
         AnimationListWidget.cpp
         AnimationPlayListWidget.cpp
         AnimationOptions.cpp
         PortMessageBox.cpp
         AnimationItem.cpp
         AnimationPropertiesPreview.cpp
    )

set(FORMS  #DebugDockWidget.ui
           MainWindow.ui
           SettingsDialog.ui
           AnimationOptions.ui
           AnimationPropertiesPreview.ui
    )

qt5_wrap_ui(${PROGNAME}_FORMS_HEADER        ${FORMS})
qt5_wrap_cpp(${PROGNAME}_HEADERS_MOC        ${HEADERS})

# or ledCube SHARED ... for shared library
add_library(ledcubegui STATIC ${GUI} ${${PROGNAME}_FORMS_HEADER} ${${PROGNAME}_HEADERS_MOC})

#message(STATUS "CMAKE_LIBRARY_PATH: " "${CMAKE_BINARY_DIR}/aqp")
# Finally link static library ledCube against QtWidget and QtSerialPort libraries
target_link_libraries(ledcubegui aqp ledcubecore)
