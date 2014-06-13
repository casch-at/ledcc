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
#   Release:    -DCMAKE_PREFIX_PATH=/PATH/TO/QT/VERSION/lib/cmake -DCMAKE_BUILD_TYPE=RELEASE
# ---------------------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------------------
# Minimum required cmake tool
# ---------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

# ---------------------------------------------------------------------------------------------------
# Specify the project name and the used programming language
# ---------------------------------------------------------------------------------------------------
project(ledcc C CXX)

# ---------------------------------------------------------------------------------------------------
# Specify executable name
# ---------------------------------------------------------------------------------------------------
set(PROGNAME ${PROJECT_NAME})

# ---------------------------------------------------------------------------------------------------
# Setup build type ( possible modes are RELEASE, DEBUG )
# ---------------------------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode")
    add_definitions(-D_DEBUG_)
else()
    message(STATUS "Release mode")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# ---------------------------------------------------------------------------------------------------
# Find packages and add Qt5 defintions
# ---------------------------------------------------------------------------------------------------
set(QT5_REQUIRED_MODULES
    Qt5Widgets
    Qt5SerialPort
    Qt5Quick
    Qt5WebKitWidgets
    Qt5LinguistTools
)

foreach(_QT5_MODULE ${QT5_REQUIRED_MODULES})
    find_package(${_QT5_MODULE} REQUIRED)
    add_definitions(${${_QT5_MODULE}_DEFINITIONS})
endforeach(_QT5_MODULE)

remove_definitions(-DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)

# ---------------------------------------------------------------------------------------------------
# Source code directories
# ---------------------------------------------------------------------------------------------------
set(CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/core")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty")
set(GUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/gui")
set(PREVIEW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/preview")
set(LANG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lang")

# ---------------------------------------------------------------------------------------------------
# Set the cmake module path to tell cmake where it has to
# look for other cmake files when include(xxx.cmake) is called
# ---------------------------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(USB1 REQUIRED)
# ---------------------------------------------------------------------------------------------------
# Setup compile flags
# ---------------------------------------------------------------------------------------------------
include(CompileFlags)

# ---------------------------------------------------------------------------------------------------
# Configure install option and cmake tool options
# ---------------------------------------------------------------------------------------------------
include(Options)

# ---------------------------------------------------------------------------------------------------
# Add the source code subdirectory
# ---------------------------------------------------------------------------------------------------
add_subdirectory( src )

# ---------------------------------------------------------------------------------------------------
# Since cmake 2.8.3 it has a build in feature summary cmake file,
# we will use it to print the used packages
# ---------------------------------------------------------------------------------------------------
set(PRINT_SUMMARY ON)
include(FeatureSummary)
feature_summary(WHAT ALL)
