#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

add_library(os_wrapper STATIC)

target_sources(os_wrapper
    PUBLIC
        os_wrapper_cmsis_rtos_v2.c
)

target_include_directories(os_wrapper PUBLIC .)

target_link_libraries(os_wrapper
    PRIVATE
        psa_interface
    PUBLIC
        tfm_nsid_manager
        RTX_OS
)
