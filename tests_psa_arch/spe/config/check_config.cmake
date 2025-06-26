#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(utils)

tfm_invalid_config((TFM_S_REG_TEST OR TFM_NS_REG_TEST) AND TEST_PSA_API)

tfm_invalid_config(SUITE STREQUAL "IPC" AND NOT TEST_PSA_API STREQUAL "IPC")

tfm_invalid_config(TEST_PSA_API STREQUAL "CRYPTO" AND NOT TFM_PARTITION_CRYPTO)
tfm_invalid_config(TEST_PSA_API STREQUAL "INITIAL_ATTESTATION" AND NOT TFM_PARTITION_INITIAL_ATTESTATION)
tfm_invalid_config(TEST_PSA_API STREQUAL "INTERNAL_TRUSTED_STORAGE" AND NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "PROTECTED_STORAGE" AND NOT TFM_PARTITION_PROTECTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "STORAGE" AND NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "STORAGE" AND NOT TFM_PARTITION_PROTECTED_STORAGE)
# PSA Arch crypto test intends to test all PSA crypto APIs. Therefore PSA Arch crypto test
# are required to use TF-M Profile profile_large.
tfm_invalid_config(TEST_PSA_API STREQUAL "CRYPTO" AND NOT (TFM_PROFILE STREQUAL "profile_large"))