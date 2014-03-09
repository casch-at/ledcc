3D-LED-Cube is a small program which implements the rs_232 interface to send data to a given µC. 
The µC than turns the leds on and off.

----------------------------------------  REQUIREMENTS --------------------------------------------------
To compile the program following requirements must be met:

Qt version >= 5
cmake version >= 2.8.10
make

--------------------------------------  COMPILE INSTRUCTION ---------------------------------------------

1) Create a build directory in the root directory and switch to it.
---
mkdir build
cd build
---
2) Create the make file with cmake, if you have not installed qt5 over the package manager 
   or you have not adjusted the system build variables you must pass the qt5 cmake path to cmake(-DCMAKE_PREFIX_PATH=/PATH/TO/QT5/lib/cmake).
--
cmake ../
if the system does not know about Qt5
cmake -DCMAKE_PREFIX_PATH=/PATH/TO/QT5/lib/cmake ../
--
3) Finally compile the program with make (replace the number with the available cores on your system or leave "-j4" away).
make -j4  

The executable should now be in the directory build/src/ , called 3D-Led-Cube.

---
One liner :-) (provided that you are already in the root directory, if not switch to the root directory
adjust the CMAKE_PREFIX_PATH, copy and paste the following line).

mkdir build && cd build && cmake -DCMAKE_PREFIX_PATH=/PATH/TO/QT5/lib/cmake ../ && make 

---
