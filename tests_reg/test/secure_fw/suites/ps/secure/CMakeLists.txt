#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_S_PS)
    return()
endif()

add_library(tfm_test_suite_ps_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_ps_s
    PRIVATE
        psa_ps_s_interface_testsuite.c
        psa_ps_s_reliability_testsuite.c
        $<$<BOOL:${PS_TEST_NV_COUNTERS}>:ps_rollback_protection_testsuite.c>
)

target_sources(tfm_app_rot_partition_ps
    PRIVATE
        $<$<BOOL:${PS_TEST_NV_COUNTERS}>:${CMAKE_CURRENT_LIST_DIR}/nv_counters/test_ps_nv_counters.c>
)

target_include_directories(tfm_test_suite_ps_s
    PUBLIC
        .
    PRIVATE
        nv_counters
)

target_compile_definitions(tfm_test_suite_ps_s
    INTERFACE
        TEST_S_PS
)

target_link_libraries(tfm_test_suite_ps_s
    PRIVATE
        tfm_test_framework_s
        platform_region_defs
        tfm_app_rot_partition_ps
        tfm_psa_rot_partition_ps_test
        tfm_spm
        platform_s
)

target_link_libraries(tfm_s_tests
    PRIVATE
        tfm_test_suite_ps_s
)
