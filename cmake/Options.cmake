# ---------------------------------------------------------------------------------------------------
# Automaticly include each directory.
# Same as adding in each cmake file -> include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
# ---------------------------------------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# ---------------------------------------------------------------------------------------------------
# Tell cmake to run moc tool when neccasery
# ---------------------------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)

option(UPDATE_TRANSLATIONS "If 'ON' .ts files will be updated, if 'OFF' .qm files will be created" OFF)

# ---------------------------------------------------------------------------------------------------
# Configure the install path
# ---------------------------------------------------------------------------------------------------
if( "${CMAKE_PREFIX_PATH}" STREQUAL  "/usr/local/")
  set(CMAKE_PREFIX_PATH "/usr/local")
elseif("${CMAKE_PREFIX_PATH}" STREQUAL "")
  set(CMAKE_PREFIX_PATH "/usr/local")
endif()
message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})

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
  set(BIN_INSTALL_DIR    "${CMAKE_PREFIX_PATH}/${CMAKE_INSTALL_BINDIR}")
  set(PLUGIN_INSTALL_DIR "${CMAKE_PREFIX_PATH}/${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
  set(DATA_INSTALL_DIR   "${CMAKE_PREFIX_PATH}/${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}" )
endif()


message(STATUS "BIN_INSTALL_DIR: ${BIN_INSTALL_DIR}" )
message(STATUS "PLUGIN_INSTALL_DIR: ${PLUGIN_INSTALL_DIR}" )
message(STATUS "DATA_INSTALL_DIR: ${DATA_INSTALL_DIR}" )


# If not debug mode set QT_NO_DEBUG
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS QT_NO_DEBUG)
endif()
