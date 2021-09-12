#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

add_library(tfm_test_framework_common INTERFACE)

target_sources(tfm_test_framework_common
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework.c
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework_helpers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework_integ_test_helper.c
)

target_include_directories(tfm_test_framework_common
    INTERFACE
        framework
)

# Build test services and add secure test suite targets
include(secure_tests.cmake)

# Include test suites at last after other targets are setup.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/suites)
