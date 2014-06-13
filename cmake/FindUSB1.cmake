# Find libusb-1.0 for portable USB support
# This module will find libusb-1.0
#
# PkgConfig will be used to find the package if present and supported,
# else search it on it own. If the LibUSB_ROOT_DIR environment variable
# is defined, it will be used as base path.
# The following standard variables get defined:
# LibUSB_FOUND:         true if LibUSB has been found
# LibUSB_VERSION:       will hold the libusb version
# LibUSB_INCLUDE_DIRS:  the directory that contains the include files
# LibUSB_LIBRARIES:     the library

include( CheckLibraryExists )
include( CheckIncludeFile )

#message(STATUS "File 'FindUSB-1.0.cmake' has been called")
find_package( PkgConfig )
if( PKG_CONFIG_FOUND )
    pkg_check_modules( USB1 libusb-1.0>=1.0.0 )
endif( PKG_CONFIG_FOUND )

if( USB1_FOUND )
#    foreach( _i ${USB1_LIBRARIES} )
#        message(STATUS "USB1_LIBRARY: ${USB1_LIBRARY} _I: ${_i}")
#        find_library( ${_i}_LIBRARY
#            NAMES ${_i}
#            PATHS ${USB1_LIBRARY_DIRS}
#        )
#        if( ${_i}_LIBRARY )
#            list( APPEND USB1_LIBRARIES ${${_i}_LIBRARY} )
#        endif( ${_i}_LIBRARY )
#        mark_as_advanced( ${_i}_LIBRARY )
#    endforeach( _i )
    set( USB1_LIBRARY ${USB1_LIBRARIES} )
    set( USB1_INCLUDE_DIR ${USB1_INCLUDE_DIRS} )

    mark_as_advanced( USB1_LIBRARY USB1_INCLUDE_DIR )

    message(STATUS "found USB1_LIBRARY: ${USB1_LIBRARY}, version = ${USB1_VERSION}")
    message(STATUS "found USB1_INCLUDE_DIR: ${USB1_INCLUDE_DIR}")
else( USB1_FOUND )

    find_path( USB1_INCLUDE_DIR
        NAMES
            libusb.h
        PATHS
            $ENV{ProgramFiles}/LIBUSB1-Win32
            $ENV{USB1_ROOT_DIR}
        PATH_SUFFIXES
            libusb-1.0
    )


    if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    # LibUSB-Win32 binary distribution contains several libs.
    # Use the lib that got compiled with the same compiler.
        if( MSVC )
            if( WIN32 )
                set( USB1_LIBRARY_PATH_SUFFIX lib/msvc )
            else( WIN32 )
                set( USB1_LIBRARY_PATH_SUFFIX lib/msvc_x64 )
            endif( WIN32 )
        elseif( BORLAND )
            set( USB1_LIBRARY_PATH_SUFFIX lib/bcc )
        elseif( CMAKE_COMPILER_IS_GNUCC )
            set( USB1_LIBRARY_PATH_SUFFIX lib/gcc )
        endif( MSVC )
    endif( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )

    find_library( USB1_LIBRARY
        NAMES
            libusb-1.0 usb-1.0
        PATHS
            $ENV{ProgramFiles}/USB1-Win32
            $ENV{USB1_ROOT_DIR}
        PATH_SUFFIXES
            ${USB1_LIBRARY_PATH_SUFFIX}
    )
    include( FindPackageHandleStandardArgs )
    
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(USB1 DEFAULT_MSG  USB1_INCLUDE_DIR USB1_LIBRARY)


    if( USB1_FOUND )
        set( USB1_LIBRARIES ${USB1_LIBRARY} )
    endif( USB1_FOUND )

    mark_as_advanced( USB1_LIBRARY USB1_INCLUDE_DIR )
endif( USB1_FOUND )

#if( LIBUSB1_FOUND )
#    set( CMAKE_REQUIRED_INCLUDES "${LibUSB_INCLUDE_DIRS}" )
#    check_include_file( libusb.h LIBUSB1_FOUND )
## message ( STATUS "LIBUSB1: libusb.h is usable: ${LIBUSB1_FOUND}" )
#endif( LIBUSB1_FOUND )

#if( LIBUSB1_FOUND )
#    check_library_exists( "${LIBUSB1_LIBRARIES}" usb_open "" LIBUSB_FOUND )
## message ( STATUS "LIBUSB1: library is usable: ${LIBUSB1_FOUND}" )
#endif( LIBUSB1_FOUND )

if( NOT USB1_FOUND )
    if( NOT USB1_FIND_QUIETLY )
        message ( STATUS "LIBUSB1 not found, try setting LIBUSB1_ROOT_DIR environment variable." )
    endif( NOT USB1_FIND_QUIETLY )
    if( USB1_FIND_REQUIRED )
        message( FATAL_ERROR "" )
    endif( USB1_FIND_REQUIRED )
endif( NOT USB1_FOUND )
