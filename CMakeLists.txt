#------------------------------------------------------------------------------
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
#------------------------------------------------------------------------------
#   If you want additional program output or want debug the program set CMAKE_BUILD_TYPE to DEBUG
#   Debug:      -DCMAKE_PREFIX_PATH=/PATH/TO/QT/VERSION/lib/cmake -DCMAKE_BUILD_TYPE=DEBUG
#   Release:    -DCMAKE_PREFIX_PATH=/PATH/TO/QT/VERSION/lib/cmake
#------------------------------------------------------------------------------
#                   Root CMakeLists file
#------------------------------------------------------------------------------
# Minimum required cmake tool
cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
# Executable name
project(ledControl C CXX)
# Variable for program name!
set(PROGNAME  ${PROJECT_NAME})

# Set version numbers for global.h configuration. See CMakeLists.txt in /src/core directory
set(LED_CUBE_VERSION_MAJOR "0.1" )
set(LED_CUBE_VERSION_MINOR "0.11")

set(CMAKE_INCLUDE_PATH)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Widgets finds its eon dependencies (QtGui and QtCore)
message(STATUS "Searching for dependencies")
find_package(Qt5Widgets REQUIRED)
find_package(Qt5SerialPort REQUIRED)

if(Qt5Widgets_FOUND AND Qt5SerialPort_FOUND AND Qt5Core_FOUND AND Qt5Gui_FOUND)
    message(STATUS "Qt5Widgets:    yes")
    message(STATUS "Qt5SerialPort: yes")
    message(STATUS "Qt5Core:       yes")
    message(STATUS "Qt5Gui:        yes")
endif()

include(CompileFlags)
#get_target_property(QtCore_location Qt5::Core LOCATION)

# Tell CMake to run moc when necessary!
set(CMAKE_AUTOMOC ON)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message(STATUS "Debug mode")
    add_definitions(-D_DEBUG_)
else()
    message(STATUS "Release mode")
endif()

if( "${CMAKE_PREFIX_PATH}" STREQUAL  "/usr/local/")
  message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
elseif("${CMAKE_PREFIX_PATH}" STREQUAL "")
  set(CMAKE_PREFIX_PATH "/usr/local")
  message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
#   message(FATAL_ERROR "No CMAKE_PREFIX_PATH specified!")
endif()

# Configure install path
if(MINGW)
  set(BIN_INSTALL_DIR    ".")
  set(PLUGIN_INSTALL_DIR ".")
  set(DATA_INSTALL_DIR   "share")
elseif(APPLE)
  set(BIN_INSTALL_DIR    ".")
  set(PLUGIN_INSTALL_DIR ".")
  set(DATA_INSTALL_DIR   "${PROGNAME}.app/Contents/Resources")
else()
  include(GNUInstallDirs)
  set(BIN_INSTALL_DIR    "${CMAKE_INSTALL_BINDIR}")
  set(PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/${PROGNAME}")
  set(DATA_INSTALL_DIR   "${CMAKE_INSTALL_DATAROOTDIR}/${PROGNAME}")
endif()

#----------- OUTPUT INSTALL DIRECTORIES
message(STATUS "BIN_INSTALL_DIR: ${BIN_INSTALL_DIR}" )
message(STATUS "PLUGIN_INSTALL_DIR: ${PLUGIN_INSTALL_DIR}" )
message(STATUS "DATA_INSTALL_DIR: ${DATA_INSTALL_DIR}" )

# If not debug mode set QT_NO_DEBUG
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS QT_NO_DEBUG)
endif()

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# Include subdirectories
include_directories(src/gui src/core/ src/core/animations src/aqp)

# Create resource file *.cpp

qt5_add_resources(${PROGNAME}_RESOURCES_RCC src/icons.qrc OPTIONS -no-compress )

add_executable(${PROGNAME} main.cpp ${${PROGNAME}_RESOURCES_RCC})
add_dependencies(${PROGNAME} ledcubegui)

# link against ledcube and animations
target_link_libraries(${PROGNAME} ledcubegui)

#qt5_use_modules(${PROGNAME} Widgets
#                            SerialPort
#)

add_subdirectory( src )
