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

#get_target_property(QtCore_location Qt5::Core LOCATION)
#--------  SET COMPILER FLAGS
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

 # set up a mapping so that the Release configuration for the Qt imported target is
 # used in the COVERAGE CMake configuration.
# set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
# set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)

if("${CMAKE_C_COMPILER}" MATCHES "clang$" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_COMPILER_IS_CLANG 1)
endif()

if("${CMAKE_CXX_COMPILER}" MATCHES "clang(\\+\\+)?$" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif()

macro(add_gcc_compiler_cxxflags FLAGS)
  if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
  endif()
endmacro(add_gcc_compiler_cxxflags)

macro(add_gcc_compiler_cflags FLAGS)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")
  endif()
endmacro(add_gcc_compiler_cflags)

macro(add_gcc_compiler_flags FLAGS)
  add_gcc_compiler_cxxflags("${FLAGS}")
  add_gcc_compiler_cflags("${FLAGS}")
endmacro(add_gcc_compiler_flags)

#add_definitions(-DQT_NO_KEYWORDS -DQT_NO_EXCEPTIONS -DQT_NO_STL -DQT_STRICT_ITERATORS -DQT_NO_CAST_TO_ASCII)
add_definitions( -DQT_NO_EXCEPTIONS -DQT_NO_STL  -DQT_NO_CAST_TO_ASCII)

#add_gcc_compiler_flags("-fno-common -fstack-protector --param=ssp-buffer-size=4 ")
add_gcc_compiler_flags("-Wall -Wextra -Wundef -Wpointer-arith -Wno-long-long")
add_gcc_compiler_flags("-Wformat=2 -Wmissing-format-attribute")
add_gcc_compiler_flags("-fvisibility=hidden")
add_gcc_compiler_cxxflags("-fvisibility-inlines-hidden")

#add_gcc_compiler_cxxflags("-fno-exceptions -fno-rtti")
add_gcc_compiler_cxxflags("-Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual")
add_gcc_compiler_cflags("-Wchar-subscripts -Wwrite-strings")

check_c_compiler_flag("-Werror=format-security -Werror=implicit-function-declaration" WERROR_C_AVAILABLE)
check_cxx_compiler_flag("-Werror=format-security" WERROR_CXX_AVAILABLE)

if(WERROR_C_AVAILABLE AND WERROR_CXX_AVAILABLE)
  add_gcc_compiler_flags("-Werror=format-security")
  add_gcc_compiler_cflags("-Werror=implicit-function-declaration")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-add-needed -Wl,--as-needed -Wl,--no-undefined")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-add-needed -Wl,--as-needed")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-z,relro")
endif()

if(WITH_LTO)
  if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)
    check_cxx_compiler_flag("-flto -fuse-linker-plugin" LTO_AVAILABLE)

    if(LTO_AVAILABLE)
      add_gcc_compiler_flags("-flto -fuse-linker-plugin")
    else()
      message(FATAL_ERROR "This version of gcc doesn't support LTO")
    endif(LTO_AVAILABLE)
  else()
    message(FATAL_ERROR "LTO is only supported with gcc")
  endif()
endif()

#if (WITH_CXX11)
#  add_gcc_compiler_cxxflags("-std=c++0x")
#  add_gcc_compiler_cflags("-ansi")
#else()
#  add_gcc_compiler_flags("-ansi")
#endif()

if(MINGW)
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()
#------------ END SET COMPILER FLAGS

if( "${CMAKE_PREFIX_PATH}" STREQUAL  "/usr/local/")
  message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
elseif("${CMAKE_PREFIX_PATH}" STREQUAL "")
  set(CMAKE_PREFIX_PATH "/usr/local")
  message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
#   message(FATAL_ERROR "No CMAKE_PREFIX_PATH specified!")
endif()



# Tell CMake to run moc when necessary!
set(CMAKE_AUTOMOC ON)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message(STATUS "Debug mode")
    add_definitions(-D_DEBUG_)
else()
    message(STATUS "Release mode")
endif()

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
message(STATUS "GNUInstallDirs: ${GNUInstallDirs}" )
message(STATUS "BIN_INSTALL_DIR: ${BIN_INSTALL_DIR}" )
message(STATUS "PLUGIN_INSTALL_DIR: ${PLUGIN_INSTALL_DIR}" )
message(STATUS "DATA_INSTALL_DIR: ${DATA_INSTALL_DIR}" )

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS QT_NO_DEBUG)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# Include subdirectories
include_directories(src/gui src/core/ src/animations src/aqp)

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
