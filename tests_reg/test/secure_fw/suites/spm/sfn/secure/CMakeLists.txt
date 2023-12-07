#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_S_SFN_BACKEND)
    return()
endif()

add_library(tfm_test_suite_sfn_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_sfn_s
    PRIVATE
        sfn_backend_s_testsuite.c
        ../sfn_backend_tests.c
        ../../common/suites/client_api_tests.c
        $<$<BOOL:${PSA_FRAMEWORK_HAS_MM_IOVEC}>:../../common/suites/mmiovec_test.c>
)

target_include_directories(tfm_test_suite_sfn_s
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_sfn_s
    INTERFACE
        TEST_S_SFN_BACKEND
)

target_link_libraries(tfm_test_suite_sfn_s
    PRIVATE
        tfm_test_framework_s
        spm_test_common
)

target_link_libraries(tfm_s_tests
    INTERFACE
        tfm_test_suite_sfn_s
)
