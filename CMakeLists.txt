#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Replace the path of this directory (CMAKE_CURRENT_SOURCE_DIR) with
# the literal "TFM_TEST_REPO_PATH" in C preprocessor macro expansions
# to prevent host file path information from leaking into __FILE__
# macros and breaking reproducible builds.
add_compile_options(
  -fmacro-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}=TFM_TEST_REPO_PATH
  )

add_subdirectory(log)

if(NS)
    add_subdirectory(app)
endif()

add_subdirectory(test)
