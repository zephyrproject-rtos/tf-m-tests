#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_sfn1 STATIC)

target_sources(tfm_app_rot_partition_sfn1
    PRIVATE
        sfn_partition1.c
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/test_services/sfn_partition1
)

# The generated sources
target_sources(tfm_app_rot_partition_sfn1
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/test_services/sfn_partition1/auto_generated/intermedia_sfn_partition1.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/test_services/sfn_partition1/auto_generated/load_info_sfn_partition1.c
)

target_include_directories(tfm_app_rot_partition_sfn1
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/test_services/sfn_partition1
        ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(tfm_app_rot_partition_sfn1
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_sprt
)

############################ Partition Defs ####################################
target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_sfn1
)
target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_SFN1
)
