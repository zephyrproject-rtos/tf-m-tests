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

add_subdirectory(log)
add_subdirectory(CMSIS)
add_subdirectory(os_wrapper)

add_library(tfm_test_broker STATIC
    main_ns.c
)

target_include_directories(tfm_test_broker PUBLIC .)

target_link_libraries(tfm_test_broker
    PUBLIC
        tfm_api_ns
        tfm_log
        os_wrapper
)

add_subdirectory(nsid_manager)
target_link_libraries(tfm_test_broker PUBLIC tfm_nsid_manager)

if (TFM_PARTITION_NS_AGENT_MAILBOX)
    add_subdirectory(multi_core)
    target_link_libraries(tfm_test_broker PUBLIC ns_multi_core)
endif()
