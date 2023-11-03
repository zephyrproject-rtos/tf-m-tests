#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(PSA_ARCH_TESTS_PATH     "DOWNLOAD"  CACHE PATH      "Path to PSA arch test repository used for SPE build")
set(PSA_ARCH_TESTS_VERSION  "v23.06_API1.5_ADAC_EAC"   CACHE STRING    "The version of PSA arch tests to use")

fetch_remote_library(
    LIB_NAME                psa_arch_tests
    LIB_SOURCE_PATH_VAR     PSA_ARCH_TESTS_PATH
    LIB_BASE_DIR            ${CMAKE_BINARY_DIR}/fetch_repo
    LIB_PATCH_DIR           ${CMAKE_CURRENT_LIST_DIR}
    LIB_FORCE_PATCH         PSA_ARCH_TESTS_FORCE_PATCH
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      https://github.com/ARM-software/psa-arch-tests.git
        GIT_TAG             ${PSA_ARCH_TESTS_VERSION}
        GIT_SHALLOW         FALSE
        GIT_PROGRESS        TRUE
        GIT_SUBMODULES      ""
)
