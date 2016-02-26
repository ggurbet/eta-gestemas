# Try to find frame on a Unix system
#
# This will define:
#
#   FRAME_FOUND       - True if frame is available
#   FRAME_LIB         - Link these to use frame
#   FRAME_INCLUDE_DIR - Include directory for frame
#   FRAME_DEFINITIONS - Compiler flags for using frame
#
# Copyright (c) 2015-2016 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
#
# Redistribution and use is allowed according to the terms of the GPLv3 license.


IF (NOT WIN32)
  IF (FRAME_INCLUDE_DIR AND FRAME_LIB)
    # In the cache already
    SET(FRAME_FIND_QUIETLY TRUE)
  ENDIF (FRAME_INCLUDE_DIR AND FRAME_LIB)

  # Use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  FIND_PACKAGE(PkgConfig)
  PKG_CHECK_MODULES(PKG_FRAME QUIET frame)

  SET(FRAME_DEFINITIONS ${PKG_FRAME_CFLAGS})

  FIND_PATH(FRAME_INCLUDE_DIR           NAMES oif/frame.h       HINTS ${PKG_FRAME_INCLUDE_DIRS})

  FIND_LIBRARY(FRAME_LIB                NAMES frame            HINTS ${PKG_XCB_LIBRARY_DIRS})

  list(REMOVE_DUPLICATES FRAME_INCLUDE_DIR)

  include(FindPackageHandleStandardArgs)

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(FRAME           DEFAULT_MSG  FRAME_LIB          FRAME_INCLUDE_DIR)

  MARK_AS_ADVANCED(
        FRAME_INCLUDE_DIR           FRAME_LIB
  )

ENDIF (NOT WIN32)
