#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_S_ITS)
    return()
endif()

add_library(tfm_test_suite_its_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_its_s
    PRIVATE
        ../its_tests_common.c
        psa_its_s_interface_testsuite.c
        psa_its_s_reliability_testsuite.c
)

target_include_directories(tfm_test_suite_its_s
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_its_s
    INTERFACE
        TEST_S_ITS
)

target_link_libraries(tfm_test_suite_its_s
    PRIVATE
        tfm_sprt
        tfm_test_framework_s
        platform_region_defs
        tfm_app_rot_partition_secure_client_2
        platform_s
)

target_link_libraries(tfm_s_tests
    PRIVATE
        tfm_test_suite_its_s
)
