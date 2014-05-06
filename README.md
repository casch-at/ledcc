ledcc
=====

3D-LED Cube Control is a small program to control a 3D-LED Cube (8x8x8) the data are send to the µC which will than turn the LEDs on and off. For the communication between the GUI and the µC the rs_232 interface is used.

IMPORTEND/TODO:: Descripe how the data are send to the µC, they need to know how to process the data. 

REQUIRMENTS
=====

Qt Version >= 5

Qt add-on module QSerialPort

cmake version >= 2.8.10


COMPILE INSTRUCTION
=====

1) Create a build directory in the root directory and switch to it.

mkdir build

cd build

2) Create the make files with cmake, if you have not installed Qt5 over the package manager
   or you have not adjusted the system environment variables you must pass the Qt5 cmake path to cmake. (-DCMAKE_PREFIX_PATH=/PATH/TO/QT5/lib/cmake)

cmake ../

Or if the Qt5 framework is installed in an none standard place type:

cmake -DCMAKE_PREFIX_PATH=/PATH/TO/QT5/lib/cmake ../

If you want install the propram to a different place (default /usr/local) you must pass the install path to cmake with --prefix=INSTALL/PATH

cmake --prefix=INSTALL/PATH ../

3) Compile the program with make (replace the number with the available cores on your system or leave "-j4" away).
make -j4

The executable should now be in the directory "build/bin/", called ledcc.

4) If you want install the program, type:

make install

Simplified
=====
One liner, provided that you are already in the root directory, if not switch to the root directory
adjust the CMAKE_PREFIX_PATH and --prefix variable to your needs.

mkdir build && cd build && cmake ../ && make && make install
