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
#   Run Cmake with: -DCMAKE_PREFIX_PATH=/opts/Qt5.2/5.2.0/gcc_64/lib/cmake/ -DCMAKE_BUILD_TYPE=DEBUG
################################################################################
#                       Create Animations Static library
################################################################################
set( SOURCES    Draw.cpp
                Animation.cpp
                Lift.cpp
                Rain.cpp
                Wall.cpp
                RandomZLift.cpp
                Firework.cpp
                WireBoxCornerShrinkGrow.cpp
                WireBoxCenterShrinkGrow.cpp
                AxisNailWall.cpp
                Loadbar.cpp
                RandomSparkFlash.cpp
                RandomSpark.cpp
                RandomFiller.cpp
                StringFly.cpp
    )

add_library( animations STATIC ${SOURCES} LookUpTables.hpp)

target_link_libraries( animations ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} )
