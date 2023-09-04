#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

###################### TF-M Secure Partition configurations ###########################
if(TEST_S_CRYPTO OR TEST_NS_CRYPTO)
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
endif()

if(TEST_S_PS OR TEST_NS_PS)
    set(TFM_PARTITION_PROTECTED_STORAGE        ON       CACHE BOOL      "Enable Protected Storage partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")

    # TEST_NS_PS relies on TEST_NS_ITS, this should goes to NS test config setup
    if(TEST_NS_PS)
        set(TEST_NS_ITS                        ON       CACHE BOOL      "Whether to build NS regression ITS tests")
    endif()
endif()

if(TEST_S_ITS OR TEST_NS_ITS)
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
endif()

if(TEST_S_ATTESTATION OR TEST_NS_ATTESTATION OR TEST_NS_T_COSE OR TEST_NS_QCBOR)
    set(TFM_PARTITION_INITIAL_ATTESTATION      ON       CACHE BOOL      "Enable Initial Attestation partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
endif()

if(TEST_S_PLATFORM OR TEST_NS_PLATFORM)
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
endif()

if((TEST_S_FWU OR TEST_NS_FWU) AND PLATFORM_HAS_FIRMWARE_UPDATE_SUPPORT)
    set(TFM_PARTITION_FIRMWARE_UPDATE          ON       CACHE BOOL      "Enable firmware update partition")
    set(TFM_PARTITION_CRYPTO                   ON       CACHE BOOL      "Enable Crypto partition")
    set(TFM_PARTITION_PLATFORM                 ON       CACHE BOOL      "Enable Platform partition")
    set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
endif()

if(TEST_NS_MANAGE_NSID AND NOT TFM_MULTI_CORE_TOPOLOGY)
    set(TFM_NS_MANAGE_NSID                     ON       CACHE BOOL      "Support NSPE OS providing NSPE client_id")
endif()

###################### Test Partition configurations ###########################
if(TEST_NS_IPC OR TEST_S_IPC)
    set(TFM_PARTITION_IPC_TEST  ON  CACHE BOOL  "Enable the IPC test Partitions")
else()
    set(TFM_PARTITION_IPC_TEST  OFF CACHE BOOL  "Enable the IPC test Partitions")
endif()

if(TEST_NS_SFN_BACKEND OR TEST_S_SFN_BACKEND)
    set(TFM_PARTITION_SFN_BACKEND_TEST  ON  CACHE BOOL  "Enable the SFN test Partitions")
else()
    set(TFM_PARTITION_SFN_BACKEND_TEST  OFF CACHE BOOL  "Enable the SFN test Partitions")
endif()

if(TEST_S_PS)
    set(TFM_PARTITION_PS_TEST ON CACHE BOOL "Enable the PS test Partition")
endif()

if(TEST_NS_SLIH_IRQ)
    set(TFM_PARTITION_SLIH_TEST ON CACHE BOOL "Enable the SLIH test Partition")
endif()

if(TEST_NS_FLIH_IRQ)
    set(TFM_PARTITION_FLIH_TEST ON CACHE BOOL "Enable the FLIH test Partition")
endif()

# Enable FPU test partition if S or NS FP test enabled
if (TEST_S_FPU OR TEST_NS_FPU)
    set(TFM_PARTITION_FPU_TEST        ON   CACHE BOOL  "Enable the FPU test Partitions")
else()
    set(TFM_PARTITION_FPU_TEST        OFF  CACHE BOOL  "Enable the FPU test Partitions")
endif()
