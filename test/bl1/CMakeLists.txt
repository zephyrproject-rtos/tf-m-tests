#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT BL1)
    return()
endif()

# Reload the compiler options for the secure core, if the NS core differs
if (EXISTS ${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload.cmake)
    tfm_toolchain_reload_compiler()
endif()

add_subdirectory(bl1_1)
add_subdirectory(bl1_2)
