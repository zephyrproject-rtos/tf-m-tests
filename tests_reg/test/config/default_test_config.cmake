#-------------------------------------------------------------------------------
# Copyright (c) 2021-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

################################## PS Tests ####################################
if (TFM_PXN_ENABLE)
    # The PS test NV counters do not work with PXN enabled
    set(PS_TEST_NV_COUNTERS     OFF        CACHE BOOL      "Use the test NV counters to test Protected Storage rollback scenarios")
else()
    set(PS_TEST_NV_COUNTERS     ON         CACHE BOOL      "Use the test NV counters to test Protected Storage rollback scenarios")
endif()

################################## Default CRYPTO Tests ########################

set(TFM_CRYPTO_TEST_ALG_CBC                     ON       CACHE BOOL      "Test CBC cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CCM                     ON       CACHE BOOL      "Test CCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CFB                     ON       CACHE BOOL      "Test CFB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_ECB                     ON       CACHE BOOL      "Test ECB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CTR                     ON       CACHE BOOL      "Test CTR cryptography mode")
set(TFM_CRYPTO_TEST_ALG_OFB                     ON       CACHE BOOL      "Test OFB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_GCM                     ON       CACHE BOOL      "Test GCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_SHA_224                 ON       CACHE BOOL      "Test SHA-224 cryptography algorithm")
if (NOT ${CC312_LEGACY_DRIVER_API_ENABLED})
    set(TFM_CRYPTO_TEST_ALG_SHA_384             OFF      CACHE BOOL      "Test SHA-384 cryptography algorithm")
    set(TFM_CRYPTO_TEST_ALG_SHA_512             OFF      CACHE BOOL      "Test SHA-512 cryptography algorithm")
else()
    set(TFM_CRYPTO_TEST_ALG_SHA_384             ON       CACHE BOOL      "Test SHA-384 cryptography algorithm")
    set(TFM_CRYPTO_TEST_ALG_SHA_512             ON       CACHE BOOL      "Test SHA-512 cryptography algorithm")
endif()
set(TFM_CRYPTO_TEST_HKDF                        ON       CACHE BOOL      "Test the HKDF key derivation algorithm")
set(TFM_CRYPTO_TEST_ECDH                        ON       CACHE BOOL      "Test the ECDH key agreement algorithm")
set(TFM_CRYPTO_TEST_CHACHA20                    OFF      CACHE BOOL      "Test the ChaCha20 stream cipher")
set(TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305       OFF      CACHE BOOL      "Test ChaCha20-Poly1305 AEAD algorithm")
set(TFM_CRYPTO_TEST_ALG_RSASSA_PSS_VERIFICATION ON       CACHE BOOL      "Test RSASSA-PSS signature verification algorithm")
set(TFM_CRYPTO_TEST_SINGLE_PART_FUNCS           ON       CACHE BOOL      "Test single-part operations in hash, MAC, AEAD and symmetric ciphers")
set(TFM_CRYPTO_TEST_UNSUPPORTED_ALG             ON       CACHE BOOL      "Test unsupported algorithm in hash, MAC")

################################## FWU Tests ###################################

set(TFM_FWU_TEST_REQUEST_REBOOT         OFF         CACHE BOOL      "Test psa_fwu_request_reboot")
set(TFM_FWU_TEST_WRITE_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_write with data block NULL")
set(TFM_FWU_TEST_QUERY_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_query with info NULL")

################################## Extra test suites ###########################

set(EXTRA_NS_TEST_SUITE_PATH            ""          CACHE PATH      "List of extra non-secure test suites directories. An extra test suite folder contains source code, CMakeLists.txt and cmake configuration file")
set(EXTRA_S_TEST_SUITE_PATH             ""          CACHE PATH      "List of extra secure test suites directories. An extra test suite folder contains source code, CMakeLists.txt and cmake configuration file")
