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

# This is important to add this SPE subdirectory at first as it
# brings configuration from TF-M build
add_subdirectory(${CONFIG_SPE_PATH} ${CMAKE_BINARY_DIR}/spe)

add_subdirectory(../log             ${CMAKE_BINARY_DIR}/log)
add_subdirectory(../CMSIS           ${CMAKE_BINARY_DIR}/CMSIS)

add_library(tfm_test_broker STATIC)

target_sources(tfm_test_broker
    PUBLIC
        ../app/main_ns.c
        ../app/os_wrapper_cmsis_rtos_v2.c
        ns_client_ext/tz_shim_layer.c
)

target_include_directories(tfm_test_broker
    PUBLIC
        .
        ../app
)

target_link_libraries(tfm_test_broker
    PUBLIC
        RTX_OS
        tfm_api_ns
        tfm_log
)

add_subdirectory(ns_client_ext)
target_link_libraries(tfm_test_broker PUBLIC tfm_nsid_manager)

if (TFM_PARTITION_NS_AGENT_MAILBOX)
    add_subdirectory(multi_core)
    target_link_libraries(tfm_test_broker PUBLIC ns_multi_core)
endif()

################################################################################
########### Patches and stubs to the existing (single) build process to leave
# legacy code untouched for backwrd compatibility.
# TODO: Sort it out after single build deprecation and code restructuring

# platform_common_interface target is linked to the log target only
add_library(platform_common_interface INTERFACE)

# platform_region_defs needed by PS tests
add_library(platform_region_defs INTERFACE)

# stub the tfm_partition target.
add_library(tfm_partitions INTERFACE)

target_link_libraries(platform_common_interface
    INTERFACE
        platform_ns
)

target_include_directories(tfm_test_broker
    PUBLIC
        ns_client_ext   # TODO: It shall be in tfm_nsid_manager. Need to sort out dependencies
)

target_include_directories(tfm_config
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
