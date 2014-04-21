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

# Configure a header file to pass some of the CMake settings to the source code
#                         -----------------------
#                          Configure "Global.hpp"
#                         -----------------------
set(LED_CUBE_VERSION_MAJOR 0 )
set(LED_CUBE_VERSION_MINOR 0)
set(LED_CUBE_VERSION_MICRO 1)
set(LED_CUBE_VERSION LED_CUBE_VERSION_MAJOR.LED_CUBE_VERSION_MINOR.LED_CUBE_VERSION_MICRO)

configure_file("Global.hpp.in" "Global.hpp" @ONLY)

