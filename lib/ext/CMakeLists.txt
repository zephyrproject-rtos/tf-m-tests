#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(CONFIG_TFM_ERPC_TEST_FRAMEWORK)
    add_subdirectory(erpc)
endif()

if(TEST_NS_ATTESTATION OR TEST_NS_QCBOR)
    add_subdirectory(qcbor)
endif()

add_subdirectory(CMSIS)
