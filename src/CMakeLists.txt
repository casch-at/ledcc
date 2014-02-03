 
# Widgets finds its own dependencies (QtGui and QtCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

include_directories (gui core aqp)

# Set Path variables
set(gui_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gui")
set(core_DIR "${CMAKE_CURRENT_SOURCE_DIR}/core")
set(aqp_DIR "${CMAKE_CURRENT_SOURCE_DIR}/aqp")
#set(cfiles_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cfiles/")


# configure a header file to pass some of the CMake settings
# to the source code
# # Configure "global.h"
# configure_file (
#   "${core_DIR}global.h.in"
#   "${core_DIR}global.h"
#   )

#message(STATUS ${${PROGNAME}_SOURCES_MAINEXE})
 # Get all files from the above set directories

set(HEADERS     ${core_DIR}/Global.hpp)
set(RESOURCES   icons.qrc)
set(MAINEXE     ../main.cpp)
set(GUI         ${gui_DIR}/DebugDockWidget.cpp
                ${gui_DIR}/MainWindow.cpp
                ${gui_DIR}/SettingsDialog.cpp
)
set(CORE        ${core_DIR}/Animations.cpp
                ${core_DIR}/Draw.cpp
)
set(AQP         ${aqp_DIR}/alt_key.cpp
                ${aqp_DIR}/aqp.cpp
                ${aqp_DIR}/kuhn_munkres.cpp
)
set(FORMS       ${gui_DIR}/DebugDockWidget.ui
                ${gui_DIR}/MainWindow.ui
                ${gui_DIR}/SettingsDialog.ui
)

#set(CFILES      ${cfiles}/AsciiHex.cpp


# PUT ALL SOURCE FILES TOGETHER
set(${PROGNAME}_SOURCES  ${GUI} ${CORE} ${AQP})

qt5_add_resources(${PROGNAME}_RESOURCES_RCC ${RESOURCES})
qt5_wrap_ui(${PROGNAME}_FORMS_HEADER        ${FORMS})
qt5_wrap_cpp(${PROGNAME}_HEADERS_MOC        ${HEADERS})

add_executable(${PROGNAME}  ${${PROGNAME}_SOURCES}
                            ${HEADERS}
                            ${MAINEXE}
                            ${${PROGNAME}_FORMS_HEADER}
                            ${${PROGNAME}_RESOURCES_RCC}
)

# The Qt5Widgets_LIBARIES variable also includes QtGui and QtCore
qt5_use_modules(${PROGNAME} Widgets 
                            SerialPort
)
                            
# Link all libraries against the program
target_link_libraries(${PROGNAME} ${Qt5Widgets_LIBRARIES}  )
