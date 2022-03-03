#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

# Add sfn test service.
if (TEST_NS_SFN_BACKEND)
    add_subdirectory(sfn_backend_test_partition)
endif()
