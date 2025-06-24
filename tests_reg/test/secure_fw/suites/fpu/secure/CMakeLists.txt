#-------------------------------------------------------------------------------
# Copyright (c) 2021-2024, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT TEST_S_FPU)
    return()
endif()

add_library(tfm_test_suite_fpu_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_fpu_s
    PRIVATE
        ../fpu_tests_common.c
        fpu_s_interface_testsuite.c
)

# Put fpu_tests_lib.c into SPRT so that both the secure tests and the test
# partition can call the functions.
target_sources(tfm_sprt
    PRIVATE
        ../fpu_tests_lib.c
)

target_include_directories(tfm_test_suite_fpu_s
    PUBLIC
        .
    PRIVATE
        ..
)

target_compile_definitions(tfm_test_suite_fpu_s
    INTERFACE
        TEST_S_FPU
)

target_link_libraries(tfm_test_suite_fpu_s
    PRIVATE
        tfm_test_framework_s
        platform_s
)

target_link_libraries(tfm_s_tests
    PRIVATE
        tfm_test_suite_fpu_s
)
