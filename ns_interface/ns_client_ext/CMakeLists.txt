#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
#
# This CMake script used in a split build only. In the legacy 'sinlge' build
# this file is ignoed. Please don't be confused.
#
cmake_minimum_required(VERSION 3.15)

if (TFM_NS_MANAGE_NSID)
    add_library(tfm_nsid_manager STATIC
            tfm_nsid_manager.c
            ../../app/tfm_nsid_map_table.c
    )

    target_compile_definitions(tfm_nsid_manager
        PUBLIC
            TFM_NS_MANAGE_NSID
            $<$<BOOL:${TEST_NS_MANAGE_NSID}>:TEST_NS_MANAGE_NSID>
    )
else()
    add_library(tfm_nsid_manager INTERFACE)
endif()
