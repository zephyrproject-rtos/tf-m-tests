#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (TFM_PARTITION_IPC_TEST OR TEST_NS_SFN_BACKEND)
    add_library(spm_test_service_common INTERFACE)

    target_include_directories(spm_test_service_common
        INTERFACE
            .
    )
endif()
