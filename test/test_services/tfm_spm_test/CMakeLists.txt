#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

# Add ipc and sfn test service.
if (TFM_PARTITION_IPC_TEST)
    add_subdirectory(tfm_ipc_client)
    add_subdirectory(tfm_ipc_service)
endif()

if (TEST_NS_SFN_BACKEND)
    add_subdirectory(sfn_partition1)
    add_subdirectory(sfn_partition2)
endif()

add_library(spm_test_service_common INTERFACE)

target_include_directories(spm_test_service_common
    INTERFACE
        common
)
