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
#  core CMakeFile ->          Configure Global.hpp
###############################################################################

# Include current directory header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Configure a header file to pass some of the CMake settings to the source code
#                         -----------------------
#                          Configure "Global.hpp"
#                         -----------------------
configure_file (
    "Global.hpp.in"
    "Global.hpp"
    @ONLY
    )

set(HEADERS Options.hpp Global.hpp)
set(SOURCES Sender.cpp Options.cpp)
add_library(ledcubecore ${SOURCES} ${HEADERS})
target_link_libraries(ledcubecore animations  ${Qt5SerialPort_LIBRARIES} )
add_dependencies(ledcubecore animations)
add_subdirectory(animations)

