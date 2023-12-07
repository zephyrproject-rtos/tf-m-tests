#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if ("${TEST_PSA_API}" STREQUAL "IPC")
    set(CONFIG_TFM_SPM_BACKEND      "IPC"       CACHE STRING    "The SPM backend [IPC, SFN]")

    configure_file(${CMAKE_CURRENT_LIST_DIR}/../tfm_psa_ff_test_manifest_list.yaml
                   ${CMAKE_BINARY_DIR}/tfm_psa_ff_test_manifest_list.yaml)
    list(APPEND TFM_EXTRA_MANIFEST_LIST_FILES ${CMAKE_BINARY_DIR}/tfm_psa_ff_test_manifest_list.yaml)
endif()

if ("${TEST_PSA_API}" STREQUAL "INITIAL_ATTESTATION")
    set(TFM_PARTITION_INITIAL_ATTESTATION      ON       CACHE BOOL      "Enable Initial Attestation partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if ("${TEST_PSA_API}" STREQUAL "CRYPTO")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if ("${TEST_PSA_API}" STREQUAL "INTERNAL_TRUSTED_STORAGE")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if ("${TEST_PSA_API}" STREQUAL "PROTECTED_STORAGE")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PROTECTED_STORAGE        ON       CACHE BOOL      "Enable Protected Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if ("${TEST_PSA_API}" STREQUAL "STORAGE")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PROTECTED_STORAGE        ON       CACHE BOOL      "Enable Protected Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

set(PROJECT_CONFIG_HEADER_FILE
    ${CMAKE_CURRENT_LIST_DIR}/../config/config_test_psa_api.h
    CACHE FILEPATH "The psa-arch-test config header file")

# Set default value for INCLUDE_PANIC_TESTS explicitly
set(INCLUDE_PANIC_TESTS     0   CACHE BOOL      "Include panic tests")
