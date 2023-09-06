#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (TFM_PARTITION_SLIH_TEST)
    add_subdirectory(tfm_slih_test_service)
endif()

if (TFM_PARTITION_FLIH_TEST)
    add_subdirectory(tfm_flih_test_service)
endif()
