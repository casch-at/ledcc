# ---------------------------------------------------------------------------------------------------
# Automaticly include each directory.
# Same as adding in each cmake file -> include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
# ---------------------------------------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# ---------------------------------------------------------------------------------------------------
# Tell cmake to run moc when neccasery
# ---------------------------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)

add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5SerialPort_DEFINITIONS})

# ---------------------------------------------------------------------------------------------------
# Setup build type ( possible modes are RELEASE, DEBUG )
# ---------------------------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message(STATUS "Debug mode")
    add_definitions(-D_DEBUG_)
else()
    message(STATUS "Release mode")
endif()


# ---------------------------------------------------------------------------------------------------
# Configure the install path
# ---------------------------------------------------------------------------------------------------
if( "${CMAKE_PREFIX_PATH}" STREQUAL  "/usr/local/")
  message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
elseif("${CMAKE_PREFIX_PATH}" STREQUAL "")
  set(CMAKE_PREFIX_PATH "/usr/local")
  message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
#   message(FATAL_ERROR "No CMAKE_PREFIX_PATH specified!")
endif()

if(MINGW)
  set(BIN_INSTALL_DIR    ".")
  set(PLUGIN_INSTALL_DIR ".")
  set(DATA_INSTALL_DIR   "share")
elseif(APPLE)
  set(BIN_INSTALL_DIR    ".")
  set(PLUGIN_INSTALL_DIR ".")
  set(DATA_INSTALL_DIR   "${PROJECT_NAME}.app/Contents/Resources")
else()
  include(GNUInstallDirs)
  set(BIN_INSTALL_DIR    "${CMAKE_INSTALL_BINDIR}")
  set(PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
  set(DATA_INSTALL_DIR   "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
endif()

# ---------------------------------------------------------------------------------------------------
# Print a message where the program will be installed
# ---------------------------------------------------------------------------------------------------
message(STATUS "BIN_INSTALL_DIR: ${BIN_INSTALL_DIR}" )
message(STATUS "PLUGIN_INSTALL_DIR: ${PLUGIN_INSTALL_DIR}" )
message(STATUS "DATA_INSTALL_DIR: ${DATA_INSTALL_DIR}" )

# If not debug mode set QT_NO_DEBUG
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS QT_NO_DEBUG)
endif()

#set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
