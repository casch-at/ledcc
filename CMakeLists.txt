# ---------------------------------------------------------------------------------------------------
#  Main CMake file
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
#   If you want additional program output or want debug the program set CMAKE_BUILD_TYPE to DEBUG
#   Debug:      -DCMAKE_PREFIX_PATH=/PATH/TO/QT/VERSION/lib/cmake -DCMAKE_BUILD_TYPE=DEBUG
#   Release:    -DCMAKE_PREFIX_PATH=/PATH/TO/QT/VERSION/lib/cmake
# ---------------------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------------------
# Minimum required cmake tool
# ---------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

# ---------------------------------------------------------------------------------------------------
# Specifie the project name and the used programming language
# ---------------------------------------------------------------------------------------------------
project(ledcc C CXX)

set(PROGNAME ${PROJECT_NAME})

# ---------------------------------------------------------------------------------------------------
# Find Qt5 Modules
# ---------------------------------------------------------------------------------------------------
set(QT_REQUIRED_MODULES Qt5Widgets Qt5Gui Qt5Core Qt5SerialPort)
foreach(QT_MODULE ${QT_REQUIRED_MODULES})
    find_package(${QT_MODULE} REQUIRED)
endforeach(QT_MODULE)

# ---------------------------------------------------------------------------------------------------
# Source code directories
# ---------------------------------------------------------------------------------------------------
set(CoreDirectory "${CMAKE_CURRENT_SOURCE_DIR}/src/core")
set(ThirdPartyDirectory "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty")
set(GuiDirectory "${CMAKE_CURRENT_SOURCE_DIR}/src/gui")


# ---------------------------------------------------------------------------------------------------
# Set the cmake module path to tell cmake where it has to look for other cmake files when include is called
# ---------------------------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ---------------------------------------------------------------------------------------------------
# Setup compile flags
# ---------------------------------------------------------------------------------------------------
include(CompileFlags)
# ---------------------------------------------------------------------------------------------------
# Configure install option and cmake tool options
# ---------------------------------------------------------------------------------------------------
include(Options)

add_subdirectory( src )
