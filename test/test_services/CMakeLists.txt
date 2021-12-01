#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Install directory for non-secure interface API of test services
# NS can pick up those interface files to build non-secure test cases
set(TEST_SERVICE_SRC_INSTALL_DIR ${TFM_INSTALL_PATH}/interface/src/test_service)
set(TEST_SERVICE_INC_INSTALL_DIR ${TFM_INSTALL_PATH}/interface/include/test_service)

if (TFM_PARTITION_ATTESTATION_TEST)
    add_subdirectory(tfm_attest_test_service)
endif()

if (TEST_NS_CORE)
    add_subdirectory(tfm_core_test)
    add_subdirectory(tfm_core_test_2)
endif()

if (TFM_PARTITION_IPC_TEST)
    add_subdirectory(tfm_ipc_client)
    add_subdirectory(tfm_ipc_service)
endif()

if (TEST_S_PS)
    add_subdirectory(tfm_ps_test_service)
endif()

if (TEST_FRAMEWORK_S)
    add_subdirectory(tfm_secure_client_service)
    add_subdirectory(tfm_secure_client_2)
endif()

if (TEST_NS_SLIH_IRQ)
    add_subdirectory(tfm_slih_test_service)
endif()

if (TEST_NS_FLIH_IRQ)
    add_subdirectory(tfm_flih_test_service)
endif()

if (TEST_S_FPU)
    add_subdirectory(tfm_fpu_service)
endif()

if (TEST_NS_SFN_BACKEND)
    add_subdirectory(tfm_test_sfn_partitions/sfn_partition1)
    add_subdirectory(tfm_test_sfn_partitions/sfn_partition2)
endif()
