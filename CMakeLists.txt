#-------------------------------------------------------------------------------
# Copyright (c) 2021-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

# This is a legacy root CMake script, kept for backward compatibility.
# In a new, split build please go to a dedicated test's subdirectory.
# For example: ./app_ns_test for building NS regression tests.

add_subdirectory(log)

add_subdirectory(lib/ext)

if(NS)
    add_subdirectory(app)
endif()

add_subdirectory(test)
