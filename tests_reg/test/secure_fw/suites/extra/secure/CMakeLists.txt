#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT EXTRA_S_TEST_SUITE_PATH)
    return()
endif()

add_library(tfm_test_suite_extra_s STATIC)

target_include_directories(tfm_test_suite_extra_s
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_extra_s
    INTERFACE
        EXTRA_S_TEST_SUITE
)

target_link_libraries(tfm_test_suite_extra_s
    PRIVATE
        tfm_test_framework_s
        platform_s
)

target_link_libraries(tfm_s_tests
    PUBLIC
        tfm_test_suite_extra_s
)

# Add extra test suites source from outside
include(extra_s_tests_config.cmake)
