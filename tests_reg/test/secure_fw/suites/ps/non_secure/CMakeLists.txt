#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_NS_PS)
    return()
endif()

add_library(tfm_test_suite_ps_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_ps_ns
    PRIVATE
        ns_test_helpers.c
        psa_ps_ns_interface_testsuite.c
)

target_include_directories(tfm_test_suite_ps_ns
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_ps_ns
    PRIVATE
        DOMAIN_NS=1

    INTERFACE
        TEST_NS_PS
)

target_link_libraries(tfm_test_suite_ps_ns
    PRIVATE
        tfm_config
        tfm_test_framework_ns
        platform_region_defs
        tfm_test_suite_its_ns
        os_wrapper
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_ps_ns
)
