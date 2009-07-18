# -*- cmake -*- -----------------------------------------------------------
# @@COPYRIGHT@@
#*-------------------------------------------------------------------------
# @file
# @brief

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
PROJECT(CML)

# Build the current CML version number:
SET(CML_MAJOR 1)
SET(CML_MINOR 1)
SET(CML_PATCH 0)
SET(CML_RELEASE_VERSION "${CML_MAJOR}.${CML_MINOR}.${CML_PATCH}")
SET(CML_FILE_VERSION "${CML_MAJOR}_${CML_MINOR}_${CML_PATCH}")

# Handy message:
MESSAGE(STATUS "Building CML ${CML_RELEASE_VERSION}")

# Include the project-specific CMake script path: */
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")


# Don't build tests by default:
OPTION(BUILD_TESTS "Build CML tests." OFF)

# Don't build examples by default:
OPTION(BUILD_EXAMPLES "Build CML examples." OFF)


# Find Boost headers and (if building tests) the static unit test framework:
IF(BUILD_TESTS)
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREAD ON)
  FIND_PACKAGE(Boost REQUIRED)
ELSE(BUILD_TESTS)
  FIND_PACKAGE(Boost REQUIRED)
ENDIF(BUILD_TESTS)
IF(Boost_FOUND)
  MESSAGE(STATUS "Found Boost ${Boost_VERSION}")
  IF(Boost_VERSION LESS 103401)
    MESSAGE(FATAL_ERROR "CML requires Boost version 1.34.1 or newer")
  ENDIF(Boost_VERSION LESS 103401)

  # Setup to use Boost headers:
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

  # Setup to use Boost libraries (currently only needed for testing):
  IF(BUILD_TESTS)
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
  ENDIF(BUILD_TESTS)
ELSE(Boost_FOUND)
  MESSAGE(FATAL_ERROR "Could not find a usable Boost version")
ENDIF(Boost_FOUND)

# Installation configuration:
IF(WIN32)

  # The paths relative to the installation path into which libraries and
  # headers should be installed for Windows builds (there are currently no
  # libraries to install):
  SET(CML_LIBARARY_PATH)
  SET(CML_HEADER_PATH)
ELSE(WIN32)

  # The paths relative to the installation path into which libraries and
  # headers should be installed for UNIX-like builds (there are currently
  # no libraries to install):
  SET(CML_LIBARARY_PATH lib)
  SET(CML_HEADER_PATH include)
ENDIF(WIN32)

# Include the source CML headers before others:
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

# Add subdirectories to build:
ADD_SUBDIRECTORY(cml)

IF(BUILD_TESTS)
  ADD_SUBDIRECTORY(tests)
ENDIF(BUILD_TESTS)

IF(BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF(BUILD_EXAMPLES)


# Setup configuration files:
CONFIGURE_FILE(
  ${CMAKE_SOURCE_DIR}/UseCML.cmake.in
  ${CMAKE_BINARY_DIR}/UseCML.cmake
  @ONLY
  )

CONFIGURE_FILE(
  ${CMAKE_SOURCE_DIR}/CMLConfig.cmake.in
  ${CMAKE_BINARY_DIR}/CMLConfig.cmake
  @ONLY
  )

INSTALL(FILES
  ${CMAKE_BINARY_DIR}/UseCML.cmake
  ${CMAKE_BINARY_DIR}/CMLConfig.cmake
  DESTINATION .
  )

# Setup CPack:
IF(WIN32 AND NOT UNIX)

  # Build ZIP and NSIS packages:
  SET(CPACK_GENERATOR "NSIS;ZIP")

  # Set the name used by NSIS to refer to the project:
  SET(CPACK_NSIS_DISPLAY_NAME "CML ${CML_RELEASE_VERSION}")
  SET(CPACK_PACKAGE_VERSION ${CML_RELEASE_VERSION})
  SET(CPACK_PACKAGE_VERSION_MAJOR ${CML_MAJOR})
  SET(CPACK_PACKAGE_VERSION_MINOR ${CML_MINOR})
  SET(CPACK_PACKAGE_VERSION_PATCH ${CML_PATCH})

  SET(CPACK_PACKAGE_FILE_NAME "cml-${CML_FILE_VERSION}")
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Configurable Math Libary")
  SET(CPACK_PACKAGE_VENDOR "Jesse Krebs, Demian Nave")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CML-${CML_FILE_VERSION}")

  #SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/Welcome.txt")
  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.rtf")
  #SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.txt")

  # Change the text at the bottom of most installer pages:
  SET(CPACK_CML_BRANDING_TEXT "www.cmldev.net - CML ${CML_RELEASE_VERSION} Installer (Nullsoft Scriptable Install System)")

  # No need to modify the user's PATH variable:
  SET(CPACK_NSIS_MODIFY_PATH OFF)

  # Done. Include CPack initialization:
  INCLUDE(CPack)
ENDIF(WIN32 AND NOT UNIX)

# --------------------------------------------------------------------------
# vim:ft=cmake
