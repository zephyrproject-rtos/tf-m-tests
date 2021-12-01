#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_S_FPU)
    return()
endif()

####################### Secure #################################################

target_sources(tfm_test_suite_fpu_s
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/fpu_tests_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/secure/fpu_s_interface_testsuite.c
)

target_include_directories(tfm_test_suite_fpu_s
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/secure
)

target_compile_definitions(tfm_test_suite_fpu_s
    INTERFACE
        TEST_S_FPU
)

target_link_libraries(tfm_test_suite_fpu_s
    PRIVATE
        tfm_test_framework_s
    PUBLIC
        platform_s
)

target_link_libraries(tfm_s_tests
    INTERFACE
        tfm_test_suite_fpu_s
)
