#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_slih_test STATIC)

add_dependencies(tfm_app_rot_partition_slih_test manifest_tool)

target_sources(tfm_app_rot_partition_slih_test
    PRIVATE
        ./tfm_slih_test_service.c
)

# The generated sources
target_sources(tfm_app_rot_partition_slih_test
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_slih_test_service/auto_generated/intermedia_tfm_slih_test_service.c
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_slih_test_service/auto_generated/load_info_tfm_slih_test_service.c
)

target_include_directories(tfm_app_rot_partition_slih_test
    PUBLIC
        ./
        ../../../common
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_slih_test_service
)

target_link_libraries(tfm_app_rot_partition_slih_test
    PRIVATE
        tfm_sprt
        platform_s
)

target_compile_definitions(tfm_app_rot_partition_slih_test
    PUBLIC
        TFM_PARTITION_SLIH_TEST
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_app_rot_partition_slih_test
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_slih_test
)

target_compile_definitions(tfm_config
    INTERFACE
        TFM_PARTITION_SLIH_TEST
)
