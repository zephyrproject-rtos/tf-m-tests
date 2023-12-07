#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_NS_ITS)
    return()
endif()

add_library(tfm_test_suite_its_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_its_ns
    PRIVATE
        ../its_tests_common.c
        psa_its_ns_interface_testsuite.c
)

target_include_directories(tfm_test_suite_its_ns
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_its_ns
    PRIVATE
        DOMAIN_NS=1
    INTERFACE
        TEST_NS_ITS
)

target_link_libraries(tfm_test_suite_its_ns
    PRIVATE
        tfm_config
        tfm_test_framework_ns
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_its_ns
)
