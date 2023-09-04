#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT DEFINED TEST_S AND NOT DEFINED TEST_NS)
    # Only when there is no user input, both test are enabled.
    set(TEST_S  ON CACHE BOOL "Enable Secure Tests")
    set(TEST_NS ON CACHE BOOL "Enable NS Tests")
endif()

# Explicitly set values
set(TEST_S  OFF CACHE BOOL "Enable Secure Tests")
set(TEST_NS OFF CACHE BOOL "Enable NS Tests")

################## Set configs that follow TEST_S/TEST_NS #########################################
# "conditional" configs
if(CONFIG_TFM_SPM_BACKEND_IPC)
    set(TEST_S_IPC          ${TEST_S}        CACHE BOOL      "Whether to build NS regression SFN backend tests")
    set(TEST_NS_IPC         ${TEST_NS}       CACHE BOOL      "Whether to build NS regression SFN backend tests")
else()
    set(TEST_S_SFN_BACKEND  ${TEST_S}        CACHE BOOL      "Whether to build S regression SFN backend tests")
    set(TEST_NS_SFN_BACKEND ${TEST_NS}       CACHE BOOL      "Whether to build S regression SFN backend tests")
endif()

# Make FLIH IRQ test as the default IRQ test
if (PLATFORM_FLIH_IRQ_TEST_SUPPORT AND NOT TEST_NS_SLIH_IRQ)
    set(TEST_NS_FLIH_IRQ    ${TEST_NS}       CACHE BOOL      "Whether to build NS regression First-Level Interrupt Handling tests")
endif()

if (PLATFORM_SLIH_IRQ_TEST_SUPPORT AND NOT TEST_NS_FLIH_IRQ)
    set(TEST_NS_SLIH_IRQ    ${TEST_NS}       CACHE BOOL      "Whether to build NS regression Second-Level Interrupt Handling tests")
endif()

# Test suites that follow the corresponding Secure Partitions
set(TEST_S_FWU          ${TFM_PARTITION_FIRMWARE_UPDATE}            CACHE BOOL  "Whether to build NS regression FWU tests")
set(TEST_NS_FWU         ${TFM_PARTITION_FIRMWARE_UPDATE}            CACHE BOOL  "Whether to build NS regression FWU tests")

set(TEST_S_ATTESTATION  ${TFM_PARTITION_INITIAL_ATTESTATION}        CACHE BOOL  "Whether to build S regression Attestation tests")
set(TEST_S_CRYPTO       ${TFM_PARTITION_CRYPTO}                     CACHE BOOL  "Whether to build S regression Crypto tests")
set(TEST_S_ITS          ${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}   CACHE BOOL  "Whether to build S regression ITS tests")
set(TEST_S_PS           ${TFM_PARTITION_PROTECTED_STORAGE}          CACHE BOOL  "Whether to build S regression PS tests")
set(TEST_S_PLATFORM     ${TFM_PARTITION_PLATFORM}                   CACHE BOOL  "Whether to build S regression Platform tests")

# Test suites that enabled by default as long as TEST_NS is ON
set(TEST_NS_ATTESTATION ${TFM_PARTITION_INITIAL_ATTESTATION}        CACHE BOOL  "Whether to build NS regression Attestation tests")
set(TEST_NS_CRYPTO      ${TFM_PARTITION_CRYPTO}                     CACHE BOOL  "Whether to build NS regression Crypto tests")
set(TEST_NS_ITS         ${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}   CACHE BOOL  "Whether to build NS regression ITS tests")
set(TEST_NS_PS          ${TFM_PARTITION_PROTECTED_STORAGE}          CACHE BOOL  "Whether to build NS regression PS tests")
set(TEST_NS_PLATFORM    ${TFM_PARTITION_PLATFORM}                   CACHE BOOL  "Whether to build NS regression Platform tests")

###################### Ensure "conditional" config has default values ##############################
set(TEST_S_IPC            OFF       CACHE BOOL      "Whether to build NS regression SFN backend tests")
set(TEST_NS_IPC           OFF       CACHE BOOL      "Whether to build NS regression SFN backend tests")
set(TEST_S_SFN_BACKEND    OFF       CACHE BOOL      "Whether to build S regression SFN backend tests")
set(TEST_NS_SFN_BACKEND   OFF       CACHE BOOL      "Whether to build S regression SFN backend tests")
set(TEST_S_FWU            OFF       CACHE BOOL      "Whether to build S regression FWU tests")
set(TEST_NS_FWU           OFF       CACHE BOOL      "Whether to build NS regression FWU tests")
set(TEST_NS_SLIH_IRQ      OFF       CACHE BOOL      "Whether to build NS regression Second-Level Interrupt Handling tests")
set(TEST_NS_FLIH_IRQ      OFF       CACHE BOOL      "Whether to build NS regression First-Level Interrupt Handling tests")

######################### Disabled test suites by default ##########################################
set(TEST_S_FPU            OFF       CACHE BOOL      "Whether to build S regression FPU tests")
set(TEST_NS_FPU           OFF       CACHE BOOL      "Whether to build NS regression FPU tests")

set(TEST_NS_MULTI_CORE    OFF       CACHE BOOL      "Whether to build NS regression multi-core tests")
set(TEST_NS_MANAGE_NSID   OFF       CACHE BOOL      "Whether to build NS regression NSID management tests")

set(TEST_NS_T_COSE        OFF       CACHE BOOL      "Whether to build NS regression t_cose tests")
set(TEST_NS_QCBOR         OFF       CACHE BOOL      "Whether to build NS regression QCBOR tests")
