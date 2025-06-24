#-------------------------------------------------------------------------------
# Copyright (c) 2020-2025, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TEST_NS_ATTESTATION AND NOT TEST_NS_T_COSE)
    return()
endif()

############################ Non Secure t_cose defs ############################
add_library(tfm_t_cose_ns_defs INTERFACE)

target_compile_definitions(tfm_t_cose_ns_defs
    INTERFACE
        T_COSE_USE_PSA_CRYPTO
        T_COSE_DISABLE_CONTENT_TYPE
        T_COSE_DISABLE_COSE_SIGN
        T_COSE_DISABLE_KEYWRAP
        T_COSE_DISABLE_PS256
        T_COSE_DISABLE_PS384
        T_COSE_DISABLE_PS512
        T_COSE_DISABLE_SHORT_CIRCUIT_SIGN
        $<$<OR:$<NOT:$<STREQUAL:${ATTEST_KEY_BITS},384>>,$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:T_COSE_DISABLE_ES384>
        $<$<OR:$<NOT:$<STREQUAL:${ATTEST_KEY_BITS},521>>,$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:T_COSE_DISABLE_ES512>
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:T_COSE_DISABLE_SIGN_VERIFY_TESTS>
)

target_include_directories(tfm_t_cose_ns_defs
    INTERFACE
        $<BUILD_INTERFACE:${T_COSE_PATH}/inc>
        $<BUILD_INTERFACE:${T_COSE_PATH}/src>
)

############################ Non Secure t_cose #################################

add_library(tfm_t_cose_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_t_cose_ns
    PRIVATE
        ${T_COSE_PATH}/src/t_cose_mac_compute.c
        ${T_COSE_PATH}/src/t_cose_mac_validate.c
        ${T_COSE_PATH}/src/t_cose_sign_sign.c
        ${T_COSE_PATH}/src/t_cose_sign1_sign.c
        ${T_COSE_PATH}/src/t_cose_signature_sign_main.c
        ${T_COSE_PATH}/src/t_cose_sign_verify.c
        ${T_COSE_PATH}/src/t_cose_sign1_verify.c
        ${T_COSE_PATH}/src/t_cose_signature_verify_main.c
        ${T_COSE_PATH}/src/t_cose_key.c
        ${T_COSE_PATH}/src/t_cose_parameters.c
        ${T_COSE_PATH}/src/t_cose_util.c
        ${T_COSE_PATH}/crypto_adapters/t_cose_psa_crypto.c
)

target_link_libraries(tfm_t_cose_ns
    PUBLIC
        tfm_t_cose_ns_defs
    PRIVATE
        tfm_api_ns
        tfm_qcbor_ns
)

if (NOT TEST_NS_T_COSE)
    return()
endif()

############################ t_cose test library ###############################

add_library(tfm_t_cose_test STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_t_cose_test
    PRIVATE
        ${T_COSE_PATH}/test/run_tests.c
        ${T_COSE_PATH}/test/t_cose_test.c
        ${T_COSE_PATH}/test/t_cose_compute_validate_mac_test.c
        $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:${T_COSE_PATH}/test/t_cose_sign_verify_test.c>
        ${T_COSE_PATH}/test/t_cose_crypto_test.c
        ${T_COSE_PATH}/examples/example_keys.c
        ${T_COSE_PATH}/examples/init_keys_psa.c
)

target_include_directories(tfm_t_cose_test
    PUBLIC
        $<BUILD_INTERFACE:${T_COSE_PATH}/test>
    PRIVATE
        $<BUILD_INTERFACE:${T_COSE_PATH}/examples>
)

target_link_libraries(tfm_t_cose_test
    PRIVATE
        tfm_api_ns
        tfm_t_cose_ns
        tfm_qcbor_ns
)

####################### Non Secure test ########################################

add_library(tfm_test_suite_t_cose_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_t_cose_ns
    PRIVATE
        t_cose_ns_testsuite.c
)

target_include_directories(tfm_test_suite_t_cose_ns
    PUBLIC
        .
)

target_compile_definitions(tfm_test_suite_t_cose_ns
    PRIVATE
        DOMAIN_NS=1
    INTERFACE
        TEST_NS_T_COSE
)

target_link_libraries(tfm_test_suite_t_cose_ns
    PRIVATE
        tfm_test_framework_ns
        tfm_t_cose_test
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_t_cose_ns
)
