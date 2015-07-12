# -*- cmake -*- -----------------------------------------------------------
# @@COPYRIGHT@@
#*-------------------------------------------------------------------------
# @file
# @brief

# Build a list of files relative to the CML source directory (e.g.
# cml/vector.h):
FILE(GLOB_RECURSE FILELIST RELATIVE ${CML_SOURCE_DIR}/cml "*.h" "*.tpp")
LIST(REMOVE_ITEM FILELIST
  code_052306_1.h
  code_061106_1.h
  )

# Make an install rule for each header:
FOREACH(Header ${FILELIST})

  # Need the directory (relative to cml/cml) containing the header:
  GET_FILENAME_COMPONENT(_path ${Header} PATH)

  # Install to _path, relative to the header installation directory:
  INSTALL(FILES ${Header} DESTINATION "${CML_HEADER_PATH}/cml/${_path}")
ENDFOREACH(Header)

# Add an interface library if cmake is version 3.1 or newer
IF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)
  ADD_LIBRARY(cml INTERFACE)
  TARGET_INCLUDE_DIRECTORIES(cml INTERFACE ${CML_SOURCE_DIR})
  TARGET_SOURCES(cml INTERFACE ${FILE_LIST})
ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)

# --------------------------------------------------------------------------
# vim:ft=cmake
