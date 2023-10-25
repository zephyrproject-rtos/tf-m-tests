#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_NS_IPC)
    return()
endif()

add_library(tfm_test_suite_ipc_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_ipc_ns
    PRIVATE
        ipc_ns_interface_testsuite.c
        ../../common/suites/client_api_tests.c
        $<$<BOOL:${PSA_FRAMEWORK_HAS_MM_IOVEC}>:../../common/suites/mmiovec_test.c>
)

target_include_directories(tfm_test_suite_ipc_ns
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_ipc_ns
    PRIVATE
        DOMAIN_NS=1
    INTERFACE
        TEST_NS_IPC
)

target_link_libraries(tfm_test_suite_ipc_ns
    PRIVATE
        tfm_test_framework_ns
        spm_test_common
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_ipc_ns
)
