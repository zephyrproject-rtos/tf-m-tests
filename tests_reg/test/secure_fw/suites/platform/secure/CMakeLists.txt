#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_S_PLATFORM)
    return()
endif()

add_library(tfm_test_suite_platform_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_platform_s
    PRIVATE
        ../platform_tests_common.c
        platform_s_interface_testsuite.c
)

target_include_directories(tfm_test_suite_platform_s
    PUBLIC
        .
    PRIVATE
        ../
)

target_compile_definitions(tfm_test_suite_platform_s
    INTERFACE
        TEST_S_PLATFORM
)

target_link_libraries(tfm_test_suite_platform_s
    PRIVATE
        tfm_test_framework_s
        platform_s
)

target_link_libraries(tfm_s_tests
    PRIVATE
        tfm_test_suite_platform_s
)
