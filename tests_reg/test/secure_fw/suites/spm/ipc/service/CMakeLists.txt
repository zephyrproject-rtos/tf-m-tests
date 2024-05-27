#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Add ipc test service.
if (TFM_PARTITION_IPC_TEST)
    add_subdirectory(tfm_ipc_client)
    add_subdirectory(tfm_ipc_service)
endif()
