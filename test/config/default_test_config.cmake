#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########################## NS test framework ###################################

set(TEST_FRAMEWORK_NS       OFF        CACHE BOOL      "Whether to build NS regression tests framework")

########################## NS test suites ######################################

set(TEST_NS_ATTESTATION     OFF        CACHE BOOL      "Whether to build NS regression Attestation tests")
set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
set(TEST_NS_QCBOR           OFF        CACHE BOOL      "Whether to build NS regression QCBOR tests")
set(TEST_NS_AUDIT           OFF        CACHE BOOL      "Whether to build NS regression Audit log tests")
set(TEST_NS_CORE            OFF        CACHE BOOL      "Whether to build NS regression Core tests")
set(TEST_NS_CRYPTO          OFF        CACHE BOOL      "Whether to build NS regression Crypto tests")
set(TEST_NS_ITS             OFF        CACHE BOOL      "Whether to build NS regression ITS tests")
set(TEST_NS_PS              OFF        CACHE BOOL      "Whether to build NS regression PS tests")
set(TEST_NS_PLATFORM        OFF        CACHE BOOL      "Whether to build NS regression Platform tests")
set(TEST_NS_FWU             OFF        CACHE BOOL      "Whether to build NS regression FWU tests")
set(TEST_NS_IPC             OFF        CACHE BOOL      "Whether to build NS regression IPC tests")
set(TEST_NS_SLIH_IRQ        OFF        CACHE BOOL      "Whether to build NS regression Second-Level Interrupt Handling tests")
set(TEST_NS_FLIH_IRQ        OFF        CACHE BOOL      "Whether to build NS regression First-Level Interrupt Handling tests")
set(TEST_NS_MULTI_CORE      OFF        CACHE BOOL      "Whether to build NS regression multi-core tests")

########################## S test framework ####################################

set(TEST_FRAMEWORK_S        OFF        CACHE BOOL      "Whether to build S regression tests framework")

########################## S test suites #######################################

set(TEST_S_ATTESTATION      OFF        CACHE BOOL      "Whether to build S regression Attestation tests")
set(TEST_S_AUDIT            OFF        CACHE BOOL      "Whether to build S regression Audit log tests")
set(TEST_S_CRYPTO           OFF        CACHE BOOL      "Whether to build S regression Crypto tests")
set(TEST_S_ITS              OFF        CACHE BOOL      "Whether to build S regression ITS tests")
set(TEST_S_PS               OFF        CACHE BOOL      "Whether to build S regression PS tests")
set(TEST_S_PLATFORM         OFF        CACHE BOOL      "Whether to build S regression Platform tests")
set(TEST_S_FWU              OFF        CACHE BOOL      "Whether to build S regression FWU tests")
set(TEST_S_IPC              OFF        CACHE BOOL      "Whether to build S regression IPC tests")

################################## Core Tests ##################################

set(TFM_INTERACTIVE_TEST                OFF         CACHE BOOL      "Enable interactive tests")

################################## PS Tests ####################################

set(PS_TEST_NV_COUNTERS                 ON          CACHE BOOL      "Use the test NV counters to test Protected Storage rollback scenarios")

################################## Default CRYPTO Tests ########################

set(TFM_CRYPTO_TEST_ALG_CBC             ON          CACHE BOOL      "Test CBC cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CCM             ON          CACHE BOOL      "Test CCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CFB             ON          CACHE BOOL      "Test CFB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_ECB             ON          CACHE BOOL      "Test ECB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CTR             ON          CACHE BOOL      "Test CTR cryptography mode")
set(TFM_CRYPTO_TEST_ALG_OFB             ON          CACHE BOOL      "Test OFB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_GCM             ON          CACHE BOOL      "Test GCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_SHA_512         ON          CACHE BOOL      "Test SHA-512 cryptography algorithm")
set(TFM_CRYPTO_TEST_HKDF                ON          CACHE BOOL      "Test the HKDF key derivation algorithm")
set(TFM_CRYPTO_TEST_ECDH                ON          CACHE BOOL      "Test the ECDH key agreement algorithm")

################################## FWU Tests ###################################

set(TFM_FWU_TEST_REQUEST_REBOOT         OFF         CACHE BOOL      "Test psa_fwu_request_reboot")
set(TFM_FWU_TEST_WRITE_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_write with data block NULL")
set(TFM_FWU_TEST_QUERY_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_query with info NULL")

################################## Initial Attestation Tests ###################

set(ATTEST_TEST_GET_PUBLIC_KEY          OFF         CACHE BOOL      "Require to retrieve Initial Attestation public in runtime for test purpose")