#-------------------------------------------------------------------------------
# Copyright (c) 2024, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT EXTRA_BL1_1_TEST_SUITE_PATH)
    return()
endif()

add_library(tfm_test_suite_extra_bl1_1 STATIC)

target_include_directories(tfm_test_suite_extra_bl1_1
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_extra_bl1_1
    INTERFACE
        EXTRA_BL1_1_TEST_SUITE
)

target_link_libraries(tfm_test_suite_extra_bl1_1
    PRIVATE
        tfm_test_framework_common
        platform_bl1_1_interface
)

target_link_libraries(bl1_1_tests
    PUBLIC
        tfm_test_suite_extra_bl1_1
)

# Add extra test suites source from outside
include(extra_bl1_1_tests_config.cmake)
