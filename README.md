# ledcc

3D-LED Cube Control is a small program to control a 3D-LED Cube (8x8x8) the data are send to the µC which will than turn the LEDs on and off. For the communication between the GUI and the µC the rs_232 interface is used.
Your implemention of the rs_232 routine on the µC site must follow a strict syntax otherwise it will not work. Checkout the [wiki](https://github.com/luxchris/ledcc/wiki) for more information. 

# REQUIRMENTS

Qt Version >= 5<br>
Qt add-on module QSerialPort<br>
cmake version >= 2.8.10<br>

# COMPILE INSTRUCTION

#### 1) Create a build directory in the root directory for out of source build.
```
$ mkdir build
$ cd build
```
#### 2) Create the make files.

```
$ cmake ../
```
##### If you have not installed Qt5 over the package manager or you have not adjusted the system environment variables you must pass the Qt5 cmake path to cmake.
```
$ cmake -DCMAKE_PREFIX_PATH=/PATH/TO/QT5/lib/cmake ../
```
##### If you want install the propram to a different place (default ```/usr/local```) you must pass the install path to cmake with:
```
$ cmake --prefix=INSTALL/PATH ../
```
#### 3) Compile the program with make.
Replace the number after ```-j``` with the available cores on your system or leave ```-j4``` away.
```
$ make -j4
```
The executable should now be in the directory ```./build/bin/```, called ledcc.

#### 4) If you want install the program, type:
```
$ make install
```
# Simplified

One liner, provided that you are already in the root directory, if not switch to the root directory and
adjust the ```CMAKE_PREFIX_PATH``` and ```--prefix``` variable to your needs.
```
$ mkdir build && cd build && cmake ../ && make && make install
```
