#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (TFM_PARTITION_SFN1)
    add_subdirectory(sfn_partition1)
endif()

if (TFM_PARTITION_SFN2)
    add_subdirectory(sfn_partition2)
endif()
