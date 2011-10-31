# Install script for directory: /Users/marheit/code/2dx/trunk/kernel/proc

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/Users/marheit/code/build/install")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/kernel/proc" TYPE FILE PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ FILES
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_checklattice_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_cleanup_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_deftilt_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_functions.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_generateMap_sub-2.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_generateMap_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_initialize_crop_histogram_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_initialize_make_image_square_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_initialize_test_endianess_of_mrc_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_initialize_tiff_to_mrc_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_merge_latline_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_merge_merge_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_merge_redoMap_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_ML_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_refine_refine_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_refine_unbend1_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_refine_unbend2_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_split.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_sym2spcgrp_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_unbend1_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_unbend2_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_unbendSyn_sub.com"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_makedirs"
    "/Users/marheit/code/2dx/trunk/kernel/proc/2dx_merge_makedirs"
    "/Users/marheit/code/2dx/trunk/kernel/proc/dummy.aph"
    "/Users/marheit/code/2dx/trunk/kernel/proc/initialize"
    "/Users/marheit/code/2dx/trunk/kernel/proc/lin"
    "/Users/marheit/code/2dx/trunk/kernel/proc/linblock"
    "/Users/marheit/code/2dx/trunk/kernel/proc/lindoubl2"
    "/Users/marheit/code/2dx/trunk/kernel/proc/lindouble"
    "/Users/marheit/code/2dx/trunk/kernel/proc/linhash"
    "/Users/marheit/code/2dx/trunk/kernel/proc/protest"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  INCLUDE("/Users/marheit/code/2dx/trunk/kernel/proc/install_symlink.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

