#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# This is default configuration file for building TF-M SPE image for S+NS tests.
# This file to be included in to TF-M build via TFM_EXTRA_CONFIG_PATH command option.
# An alternative file can be pased for S test via CONFIG_TFM_REG_TESTS argument.
#-------------------------------------------------------------------------------

set(TEST_S                  ON       CACHE BOOL      "Whether to build S regression tests")
set(TFM_S_REG_TEST          ON       CACHE BOOL      "Enable S regression test")
set(NS                      OFF      CACHE BOOL      "Enalbe NS side build")

set(TEST_S_ATTESTATION      ON       CACHE BOOL      "Whether to build S regression Attestation tests")
set(TEST_S_CRYPTO           ON       CACHE BOOL      "Whether to build S regression Crypto tests")
set(TEST_S_ITS              ON       CACHE BOOL      "Whether to build S regression ITS tests")
set(TEST_S_PS               ON       CACHE BOOL      "Whether to build S regression PS tests")
set(TEST_S_PLATFORM         ON       CACHE BOOL      "Whether to build S regression Platform tests")
set(TEST_S_SFN_BACKEND      ON       CACHE BOOL      "Whether to build S regression SFN tests")

set(TEST_S_FWU              OFF      CACHE BOOL      "Whether to build S regression FWU tests")
set(TEST_S_IPC              OFF      CACHE BOOL      "Whether to build S regression IPC tests")
set(TEST_S_FPU              OFF      CACHE BOOL      "Whether to build S regression FPU tests")

#-------------------------------------------------------------------------------
# Use local repositories and avoid fetching them on evey clean build.
# A tempoarl settings to be remove later.
#-------------------------------------------------------------------------------
if(EXISTS ${CMAKE_SOURCE_DIR}/localrepos.cmake)
    include(${CMAKE_SOURCE_DIR}/localrepos.cmake)
endif()
