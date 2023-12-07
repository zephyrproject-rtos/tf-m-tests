#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_S_IPC)
    return()
endif()

add_library(tfm_test_suite_ipc_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_ipc_s
    PRIVATE
        ipc_s_interface_testsuite.c
        ../../common/suites/client_api_tests.c
)

target_include_directories(tfm_test_suite_ipc_s
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_ipc_s
    PRIVATE
        $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<BOOL:${TFM_ISOLATION_TEST_APP_2_PSA}>>:TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM>
        $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<BOOL:${TFM_ISOLATION_TEST_MEM_CHECK}>>:TFM_IPC_ISOLATION_2_MEM_CHECK>
    INTERFACE
        TEST_S_IPC
)

target_link_libraries(tfm_test_suite_ipc_s
    PRIVATE
        tfm_test_framework_s
        spm_test_common
)

target_link_libraries(tfm_s_tests
    PRIVATE
        tfm_test_suite_ipc_s
)
