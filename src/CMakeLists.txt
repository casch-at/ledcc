 
# Widgets finds its own dependencies (QtGui and QtCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

include_directories (gui core aqp cfiles)

# Set Path variables
set(gui_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gui/")
# set(usb_DIR "${CMAKE_CURRENT_SOURCE_DIR}/usb/")
set(core_DIR "${CMAKE_CURRENT_SOURCE_DIR}/core/")
set(aqp_DIR "${CMAKE_CURRENT_SOURCE_DIR}/aqp/")
#set(cfiles_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cfiles/")
# configure a header file to pass some of the CMake settings
# to the source code

# # Configure "global.h"
# configure_file (
#   "${core_DIR}global.h.in"
#   "${core_DIR}global.h"
#   )
set(${PROGNAME}_SOURCES_MAINEXE
    ../main.cpp
)
message(STATUS ${${PROGNAME}_SOURCES_MAINEXE})

set(${PROGNAME}_HEADERS
    ${core_DIR}Global.hpp
#    ${cfiles_DIR}ledCubeAnimations.h
)
set(${PROGNAME}_RESOURCES
    icons.qrc
)

 # Get all files from the above set directories
file (GLOB GUI
    "${gui_DIR}*.cpp"
)

file (GLOB CORE
    "${core_DIR}*.cpp"
)

file (GLOB AQP
    "${aqp_DIR}*.cpp"
)

#file (GLOB CFILES
#     "${cfiles_DIR}*.c"
#)
file(GLOB ${PROGNAME}_FORMS
    "${gui_DIR}*.ui"
)

# REMOVE UNEEDED FILES
LIST(REMOVE_ITEM AQP
    "${aqp_DIR}main.cpp"
)

LIST(REMOVE_ITEM ${PROGNAME}_FORMS
    "${gui_DIR}mainwindow_bak.ui"
)

LIST(REMOVE_ITEM ${PROGNAME}_FORMS
    "${gui_DIR}AddDialog.ui"
)

LIST(REMOVE_ITEM GUI
    "${gui_DIR}AddDialog.cpp"
)

# PUT ALL SOURCE FILES TOGETHER
set(${PROGNAME}_SOURCES
    ${GUI}
    ${CORE}
    ${CFILES}
    ${AQP}
)


set(${PROGNAME}_HEADER    ${${PROGNAME}_HEADERS})

qt5_add_resources(${PROGNAME}_RESOURCES_RCC ${${PROGNAME}_RESOURCES})

qt5_wrap_ui(${PROGNAME}_FORMS_HEADER ${${PROGNAME}_FORMS})
qt5_wrap_cpp(${PROGNAME}_HEADERS_MOC ${${PROGNAME}_HEADER})

add_executable(${PROGNAME}  ${${PROGNAME}_SOURCES}
                            ${${PROGNAME}_HEADER}
                            ${${PROGNAME}_SOURCES_MAINEXE}
                            ${${PROGNAME}_FORMS_HEADER}
                            ${${PROGNAME}_RESOURCES_RCC}
)

# The Qt5Widgets_LIBARIES variable also includes QtGui and QtCore
qt5_use_modules(${PROGNAME} Widgets 
                            SerialPort
)
                            
# Link all libraries against the program
target_link_libraries(${PROGNAME} ${Qt5Widgets_LIBRARIES}  )
