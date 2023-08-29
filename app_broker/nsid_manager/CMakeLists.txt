#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_nsid_manager STATIC)

target_include_directories(tfm_nsid_manager PUBLIC .)

target_link_libraries(tfm_nsid_manager PRIVATE RTX_OS)

if(TFM_NS_MANAGE_NSID)
    target_sources(tfm_nsid_manager
        PUBLIC
            tfm_nsid_manager.c
            tfm_nsid_map_table.c
            tz_shim_layer.c
    )

    target_compile_definitions(tfm_nsid_manager
        PUBLIC
            TFM_NS_MANAGE_NSID
            $<$<BOOL:${TEST_NS_MANAGE_NSID}>:TEST_NS_MANAGE_NSID>
    )
else()
    target_sources(tfm_nsid_manager
        PUBLIC
            tz_stub.c
    )
endif()
