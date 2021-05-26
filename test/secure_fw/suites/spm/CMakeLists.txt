#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

# Add ipc and sfn test suite.
add_subdirectory(ipc)
add_subdirectory(sfn)

add_library(spm_test_common INTERFACE)

target_include_directories(spm_test_common
    INTERFACE
        common
        ../../test_services/tfm_spm_test
        ../../test_services/tfm_spm_test/common
)
