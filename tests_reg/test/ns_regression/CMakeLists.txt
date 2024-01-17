#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

########################## TEST Configuration ##################################

include(utils)
    dump_options("NS TEST Configuration"
    "
        TEST_NS;
        TEST_NS_ATTESTATION;
        TEST_NS_CRYPTO;
        TEST_NS_ITS;
        TEST_NS_PS;
        TEST_NS_QCBOR;
        TEST_NS_T_COSE;
        TEST_NS_PLATFORM;
        TEST_NS_FWU;
        TEST_NS_IPC;
        TEST_NS_SLIH_IRQ;
        TEST_NS_FLIH_IRQ;
        TEST_NS_MULTI_CORE;
        TEST_NS_MANAGE_NSID;
        TEST_NS_SFN_BACKEND;
        TEST_NS_FPU;
    "
    )

# Common regression test framework
add_subdirectory(${CMAKE_SOURCE_DIR}/test/framework ${CMAKE_CURRENT_BINARY_DIR}/framework)

add_library(tfm_ns_tests INTERFACE)
add_library(tfm_test_framework_ns INTERFACE)

target_compile_definitions(tfm_test_framework_ns
    INTERFACE
        DOMAIN_NS=1
)

target_link_libraries(tfm_test_framework_ns
    INTERFACE
        tfm_test_framework_common
        tfm_api_ns
        tfm_ns_log
)

target_sources(tfm_ns_tests
    INTERFACE
        non_secure_suites.c
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_framework_ns
        tfm_config
)

target_compile_definitions(tfm_ns_tests
    INTERFACE
        $<$<BOOL:${TFM_FUZZER_TOOL_TESTS}>:TFM_FUZZER_TOOL_TESTS>
)

add_subdirectory(${CMAKE_SOURCE_DIR}/test/secure_fw/non_secure
                 ${CMAKE_CURRENT_BINARY_DIR}/secure_fw/suites
)
