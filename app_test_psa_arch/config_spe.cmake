#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
#
# This is a configuration files for building SPE, necessary for PSA API Arch tests.
# It shall be passed to SPE build via TFM_EXTRA_CONFIG_PATH option.
#-------------------------------------------------------------------------------

# Do not forget to specify configuration file While building SPE via:
# -DPROJECT_CONFIG_HEADER_FILE=<>/config_test_psa_api.h

set(TEST_S                  OFF      CACHE BOOL      "Whether to build S regression tests")
set(TFM_S_REG_TEST          OFF      CACHE BOOL      "Enable S regression test")
set(NS                      OFF      CACHE BOOL      "Enalbe NS side build")

if ("${TEST_PSA_API}" STREQUAL "IPC")
    # PSA Arch test partitions only support IPC model so far
    set(CONFIG_TFM_SPM_BACKEND      "IPC"       CACHE STRING    "The SPM backend [IPC, SFN]")
    set(TFM_PARTITION_FF_TEST   ON)
else()
    set(TFM_PARTITION_FF_TEST   OFF)
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

if ("${TEST_PSA_API}" STREQUAL "STORAGE")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_PROTECTED_STORAGE        ON       CACHE BOOL      "Enable Protected Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()
