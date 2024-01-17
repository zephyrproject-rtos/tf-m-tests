#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

# Add test suites and test service common library
add_subdirectory(../common/suites   ${CMAKE_CURRENT_BINARY_DIR}/common/suites)

# Add test suites.
add_subdirectory(../ipc/non_secure  ${CMAKE_CURRENT_BINARY_DIR}/ipc)
add_subdirectory(../sfn/non_secure  ${CMAKE_CURRENT_BINARY_DIR}/sfn)
add_subdirectory(../irq/non_secure  ${CMAKE_CURRENT_BINARY_DIR}/irq)
