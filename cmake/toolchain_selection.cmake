#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Select toolchain file if it is not specified via command line or the absolutate path
# is unavailable.

if (NOT DEFINED TFM_TOOLCHAIN_FILE)
    set(TFM_TOOLCHAIN_FILE    ${CONFIG_SPE_PATH}/cmake/toolchain_ns_GNUARM.cmake)
endif()

if(NOT IS_ABSOLUTE ${TFM_TOOLCHAIN_FILE})
    get_filename_component(RELATIVE_DIR ${TFM_TOOLCHAIN_FILE} DIRECTORY)
    if("${RELATIVE_DIR}" STREQUAL "")
        # Assume the toolchain is put in ${CONFIG_SPE_PATH}/cmake
        set(TFM_TOOLCHAIN_FILE "${CONFIG_SPE_PATH}/cmake/${TFM_TOOLCHAIN_FILE}")
    else()
        # Assume the relative path is based on ${CONFIG_SPE_PATH}
        set(TFM_TOOLCHAIN_FILE "${CONFIG_SPE_PATH}/${TFM_TOOLCHAIN_FILE}")
    endif()
endif()

if(NOT EXISTS ${TFM_TOOLCHAIN_FILE})
    message(FATAL_ERROR "TFM_TOOLCHAIN_FILE ${TFM_TOOLCHAIN_FILE} doesn't exist")
endif()
