#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_policy(SET CMP0079 NEW)

####################### qcbor ns ###############################################

add_library(tfm_qcbor_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor_ns
    PRIVATE
        ${QCBOR_PATH}/src/ieee754.c
        ${QCBOR_PATH}/src/qcbor_encode.c
        ${QCBOR_PATH}/src/qcbor_decode.c
        ${QCBOR_PATH}/src/UsefulBuf.c
)

target_include_directories(tfm_qcbor_ns
    PUBLIC
        $<BUILD_INTERFACE:${QCBOR_PATH}/inc>
    INTERFACE
        $<BUILD_INTERFACE:${QCBOR_PATH}/inc/qcbor>
)

target_link_libraries(tfm_qcbor_ns
    PRIVATE
        tfm_t_cose_defs
)

target_compile_options(tfm_qcbor_ns
    PUBLIC
        ${COMPILER_CP_FLAG}
)

target_compile_definitions(tfm_qcbor_ns
    PUBLIC
        QCBOR_DISABLE_FLOAT_HW_USE
        USEFULBUF_DISABLE_ALL_FLOAT
        QCBOR_DISABLE_PREFERRED_FLOAT
)

####################### qcbor test #############################################

add_library(tfm_qcbor_test STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor_test
    PRIVATE
        ${QCBOR_PATH}/test/qcbor_decode_tests.c
        ${QCBOR_PATH}/test/qcbor_encode_tests.c
        ${QCBOR_PATH}/test/run_tests.c
        ${QCBOR_PATH}/test/UsefulBuf_Tests.c
)

target_include_directories(tfm_qcbor_test
    PUBLIC
        $<BUILD_INTERFACE:${QCBOR_PATH}/test>
    PRIVATE
        $<BUILD_INTERFACE:${QCBOR_PATH}/inc>
)

target_link_libraries(tfm_qcbor_test
    PRIVATE
        tfm_qcbor_ns
)

####################### Non Secure test ########################################

if (NOT TEST_NS_QCBOR)
    return()
endif()

add_library(tfm_test_suite_qcbor_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_qcbor_ns
    PRIVATE
        non_secure/qcbor_ns_testsuite.c
)

target_include_directories(tfm_test_suite_qcbor_ns
    PUBLIC
        non_secure
        $<BUILD_INTERFACE:${QCBOR_PATH}/test>
)

target_compile_definitions(tfm_test_suite_qcbor_ns
    PRIVATE
        DOMAIN_NS=1
    INTERFACE
        TEST_NS_QCBOR
)

target_link_libraries(tfm_test_suite_qcbor_ns
    PRIVATE
        tfm_test_framework_ns
        tfm_qcbor_test
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_qcbor_ns
)
