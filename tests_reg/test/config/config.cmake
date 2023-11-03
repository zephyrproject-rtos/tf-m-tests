#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/enable_dep_config.cmake)

########################## TEST SYNC ###########################################

if ((NOT TFM_PARTITION_PROTECTED_STORAGE))
    set(TEST_NS_PS              OFF        CACHE BOOL      "Whether to build NS regression PS tests")
    set(TEST_S_PS               OFF        CACHE BOOL      "Whether to build S regression PS tests")
endif()

if (TFM_PXN_ENABLE)
    # The PS test NV counters do not work with PXN enabled
    set(PS_TEST_NV_COUNTERS     OFF        CACHE BOOL      "Use the test NV counters to test Protected Storage rollback scenarios")
endif()

if (NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
    set(TEST_NS_ITS             OFF        CACHE BOOL      "Whether to build NS regression ITS tests")
    set(TEST_S_ITS              OFF        CACHE BOOL      "Whether to build S regression ITS tests")

    # TEST_NS_PS relies on TEST_NS_ITS
    set(TEST_NS_PS              OFF        CACHE BOOL      "Whether to build NS regression PS tests")
endif()

if (NOT TFM_PARTITION_CRYPTO)
    set(TEST_NS_CRYPTO          OFF        CACHE BOOL      "Whether to build NS regression Crypto tests")
    set(TEST_S_CRYPTO           OFF        CACHE BOOL      "Whether to build S regression Crypto tests")
endif()

if (NOT TFM_PARTITION_INITIAL_ATTESTATION)
    set(TEST_NS_ATTESTATION     OFF        CACHE BOOL      "Whether to build NS regression Attestation tests")
    set(TEST_S_ATTESTATION      OFF        CACHE BOOL      "Whether to build S regression Attestation tests")
    set(TEST_NS_QCBOR           OFF        CACHE BOOL      "Whether to build NS regression QCBOR tests")
    set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
endif()

if (SYMMETRIC_INITIAL_ATTESTATION)
    set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
endif()

if (NOT TFM_PARTITION_PLATFORM)
    set(TEST_NS_PLATFORM        OFF        CACHE BOOL      "Whether to build NS regression Platform tests")
    set(TEST_S_PLATFORM         OFF        CACHE BOOL      "Whether to build S regression Platform tests")
endif()

if (NOT TFM_PARTITION_FIRMWARE_UPDATE)
    set(TEST_NS_FWU             OFF        CACHE BOOL      "Whether to build NS regression FWU tests")
    set(TEST_S_FWU              OFF        CACHE BOOL      "Whether to build S regression FWU tests")
endif()

if (NOT TFM_MULTI_CORE_TOPOLOGY)
    set(TEST_NS_MULTI_CORE      OFF        CACHE BOOL      "Whether to build NS regression multi-core tests")
endif()

if (NOT TFM_NS_MANAGE_NSID)
    set(TEST_NS_MANAGE_NSID     OFF        CACHE BOOL      "Whether to build NS regression NSID management tests")
endif()

if (CONFIG_TFM_FLOAT_ABI STREQUAL "soft")
    set(TEST_S_FPU              OFF        CACHE BOOL      "Whether to build S regression FPU tests")
    set(TEST_NS_FPU             OFF        CACHE BOOL      "Whether to build NS regression FPU tests")
endif()

########################## Test profile ########################################

if (TFM_PROFILE)
    include(${CMAKE_CURRENT_LIST_DIR}/profile/${TFM_PROFILE}_test.cmake)
endif()

########################## SLIH/FLIH IRQ Test ##################################

# Make FLIH IRQ test as the default IRQ test
if (PLATFORM_FLIH_IRQ_TEST_SUPPORT
    AND TEST_NS AND NOT TEST_NS_SLIH_IRQ)
    set(TEST_NS_FLIH_IRQ        ON        CACHE BOOL      "Whether to build NS regression First-Level Interrupt Handling tests")
endif()

if (PLATFORM_SLIH_IRQ_TEST_SUPPORT
    AND TEST_NS AND NOT TEST_NS_FLIH_IRQ)
    set(TEST_NS_SLIH_IRQ        ON        CACHE BOOL      "Whether to build NS regression Second-Level Interrupt Handling tests")
endif()

############################ IPC backend test ##################################
if (CONFIG_TFM_SPM_BACKEND_IPC AND TEST_NS)
    set(TEST_NS_IPC             ON        CACHE BOOL      "Whether to build NS regression SFN backend tests")
endif()

if (CONFIG_TFM_SPM_BACKEND_IPC AND TEST_S)
    set(TEST_S_IPC              ON        CACHE BOOL      "Whether to build NS regression SFN backend tests")
endif()

############################ SFN backend test ##################################

if (CONFIG_TFM_SPM_BACKEND_SFN AND TEST_NS)
    set(TEST_NS_SFN_BACKEND     ON        CACHE BOOL      "Whether to build NS regression SFN backend tests")
endif()

if (CONFIG_TFM_SPM_BACKEND_SFN AND TEST_S)
    set(TEST_S_SFN_BACKEND      ON        CACHE BOOL      "Whether to build S regression SFN backend tests")
endif()

########################## Load default config #################################

if (TEST_S)
    include(${CMAKE_CURRENT_LIST_DIR}/default_s_test_config.cmake)
endif()
if (TEST_NS)
    include(${CMAKE_CURRENT_LIST_DIR}/default_ns_test_config.cmake)
endif()

###################### Test Partition configurations ###########################
if (TEST_NS_IPC OR TEST_S_IPC)
    set(TFM_PARTITION_IPC_TEST  ON)
else()
    set(TFM_PARTITION_IPC_TEST  OFF)
endif()

if (TEST_NS_PS OR TEST_S_PS)
    set(TFM_PARTITION_PS_TEST  ON)
else()
    set(TFM_PARTITION_PS_TEST  OFF)
endif()

if (TEST_NS_SFN_BACKEND OR TEST_S_SFN_BACKEND)
    set(TFM_PARTITION_SFN_BACKEND_TEST  ON)
else()
    set(TFM_PARTITION_SFN_BACKEND_TEST  OFF)
endif()

# Enable FPU test partition if S or NS FP test enabled
if (TEST_S_FPU OR TEST_NS_FPU)
    set(TFM_PARTITION_FPU_TEST          ON)
else()
    set(TFM_PARTITION_FPU_TEST          OFF)
endif()

if(TEST_NS_FLIH_IRQ)
    set(TFM_PARTITION_FLIH_TEST        ON)
else()
    set(TFM_PARTITION_FLIH_TEST        OFF)
endif()

if(TEST_NS_SLIH_IRQ)
    set(TFM_PARTITION_SLIH_TEST        ON)
else()
    set(TFM_PARTITION_SLIH_TEST        OFF)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/default_test_config.cmake)

# Test Secure Partition building using out-of-tree build
list(APPEND TFM_EXTRA_MANIFEST_LIST_FILES ${CONFIG_TFM_TEST_DIR}/../secure_fw/tfm_test_manifest_list.yaml)

set(SECURE_FW_REG_DIR ${CMAKE_CURRENT_LIST_DIR}/../secure_fw)

# Test services are also required by some NS regression tests.
# Include test services at first no matter whether secure tests are enabled.
list(APPEND TFM_EXTRA_PARTITION_PATHS
     ${SECURE_FW_REG_DIR}/suites/spm/ipc/service
     ${SECURE_FW_REG_DIR}/suites/spm/sfn/service
     ${SECURE_FW_REG_DIR}/suites/spm/irq/service
     ${SECURE_FW_REG_DIR}/suites/ps/service
     ${SECURE_FW_REG_DIR}/suites/fpu/service
)

if(TFM_S_REG_TEST)
    # secure test services are required if any secure test is opened
    list(APPEND TFM_EXTRA_PARTITION_PATHS
         ${SECURE_FW_REG_DIR}/common_test_services/tfm_secure_client_service
         ${SECURE_FW_REG_DIR}/common_test_services/tfm_secure_client_2
)
endif()
