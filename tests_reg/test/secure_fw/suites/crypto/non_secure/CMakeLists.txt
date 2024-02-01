#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

####################### Non Secure #############################################

if(NOT TEST_NS_CRYPTO)
    return()
endif()

add_library(tfm_test_suite_crypto_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_crypto_ns
    PRIVATE
        ../crypto_tests_common.c
        crypto_ns_interface_testsuite.c
)

target_include_directories(tfm_test_suite_crypto_ns
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(tfm_test_suite_crypto_ns
    PRIVATE
        tfm_config
        tfm_test_framework_ns
        psa_crypto_config
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_crypto_ns
)

target_compile_definitions(tfm_test_suite_crypto_ns
    PRIVATE
        DOMAIN_NS=1
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_CBC}>:TFM_CRYPTO_TEST_ALG_CBC>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_CCM}>:TFM_CRYPTO_TEST_ALG_CCM>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_CFB}>:TFM_CRYPTO_TEST_ALG_CFB>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_ECB}>:TFM_CRYPTO_TEST_ALG_ECB>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_CTR}>:TFM_CRYPTO_TEST_ALG_CTR>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_OFB}>:TFM_CRYPTO_TEST_ALG_OFB>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_GCM}>:TFM_CRYPTO_TEST_ALG_GCM>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_SHA_224}>:TFM_CRYPTO_TEST_ALG_SHA_224>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_SHA_384}>:TFM_CRYPTO_TEST_ALG_SHA_384>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_SHA_512}>:TFM_CRYPTO_TEST_ALG_SHA_512>
        $<$<BOOL:${TFM_CRYPTO_TEST_HKDF}>:TFM_CRYPTO_TEST_HKDF>
        $<$<BOOL:${TFM_CRYPTO_TEST_ECDH}>:TFM_CRYPTO_TEST_ECDH>
        $<$<BOOL:${TFM_CRYPTO_TEST_CHACHA20}>:TFM_CRYPTO_TEST_CHACHA20>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305}>:TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305>
        $<$<BOOL:${TFM_CRYPTO_TEST_ALG_RSASSA_PSS_VERIFICATION}>:TFM_CRYPTO_TEST_ALG_RSASSA_PSS_VERIFICATION>
        $<$<BOOL:${TFM_CRYPTO_TEST_SINGLE_PART_FUNCS}>:TFM_CRYPTO_TEST_SINGLE_PART_FUNCS>
        $<$<BOOL:${TFM_CRYPTO_TEST_UNSUPPORTED_ALG}>:TFM_CRYPTO_TEST_UNSUPPORTED_ALG>
    INTERFACE
        TEST_NS_CRYPTO
)
