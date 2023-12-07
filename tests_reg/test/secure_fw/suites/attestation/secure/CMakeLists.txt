#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(NOT TEST_S_ATTESTATION)
    return()
endif()

add_library(tfm_test_suite_attestation_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_attestation_s
    PRIVATE
        ../attest_token_test.c
        ../attest_token_decode_common.c
        ../ext/qcbor_util/qcbor_util.c
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:../attest_token_decode_symmetric.c>
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:attest_symmetric_s_interface_testsuite.c>
        $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:../attest_token_decode_asymmetric.c>
        $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:attest_asymmetric_s_interface_testsuite.c>
)

target_include_directories(tfm_test_suite_attestation_s
    PUBLIC
        .
        ../ext/qcbor_util
    PRIVATE
        ..
)

target_compile_definitions(tfm_test_suite_attestation_s
    PRIVATE
        $<$<BOOL:${ATTEST_INCLUDE_TEST_CODE}>:INCLUDE_TEST_CODE>
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
    INTERFACE
        TEST_S_ATTESTATION
)

target_link_libraries(tfm_test_suite_attestation_s
    PRIVATE
        qcbor
        tfm_t_cose_s
        tfm_test_framework_s
        tfm_attestation_defs
        tfm_boot_status
        tfm_psa_rot_partition_crypto
        platform_s
)

target_link_libraries(tfm_s_tests
    PRIVATE
        tfm_test_suite_attestation_s
)
