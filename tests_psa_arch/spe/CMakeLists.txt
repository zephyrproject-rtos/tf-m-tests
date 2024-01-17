#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../cmake)
include(utils)

# Force cross compilation and avoid compiler search and test
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

project("TF-M SPE for PSA tests" LANGUAGES C)

# use tf-m-tests/cmake folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)
include(remote_library)

# User must specify which TEST_PSA_API suit name
if (NOT DEFINED TEST_PSA_API)
    message(FATAL_ERROR "TEST_PSA_API is not defined. Please provide desired test suit name provide.")
endif()

if (NOT DEFINED CONFIG_TFM_SOURCE_PATH OR NOT EXISTS ${CONFIG_TFM_SOURCE_PATH})
    message(FATAL_ERROR "CONFIG_TFM_SOURCE_PATH = ${CONFIG_TFM_SOURCE_PATH} is not defined or incorrect. Please provide full path to TF-M sources.")
endif()

if (NOT DEFINED PSA_ARCH_TESTS_PATH OR NOT EXISTS ${PSA_ARCH_TESTS_PATH})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../fetch_repo ${CMAKE_BINARY_DIR}/fetch_repo)
endif()

collect_build_cmd_args(TFM_CMDLINE_CONFIGS)

if ("${TEST_PSA_API}" STREQUAL "IPC")
    # Workaround: amend IPC backend config to command-line options so that users (including CI)
    # do not need to set backend for FF tests.
    # This should be done in the config_test_psa_api.cmake but it is included by TF-M too late.
    # So setting backend in it does not take effect.
    set(TFM_CMDLINE_CONFIGS "${TFM_CMDLINE_CONFIGS};-DCONFIG_TFM_SPM_BACKEND:UNINITIALIZED=IPC")
endif()

#------------------------------------------------------------------------------

# tfm_s_test is IMPORTED to inform CMake that it has no source files.
add_executable(tfm_test_spe IMPORTED)

include(ExternalProject)

ExternalProject_Add(TF-M
    SOURCE_DIR        ${CONFIG_TFM_SOURCE_PATH}
    BINARY_DIR        build-spe
    INSTALL_DIR       api_ns
    CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    CMAKE_ARGS        -DPSA_ARCH_TESTS_PATH=${PSA_ARCH_TESTS_PATH}
    CMAKE_ARGS        -DCONFIG_TFM_TEST_DIR=${CMAKE_SOURCE_DIR}/partitions
    CMAKE_ARGS        -DCONFIG_PSA_ARCH_TESTS_CONFIG_FILE=${CMAKE_CURRENT_LIST_DIR}/config/config_test_psa_api.cmake
    CMAKE_CACHE_DEFAULT_ARGS ${TFM_CMDLINE_CONFIGS}
    PREFIX             "temp"
    BUILD_ALWAYS       ON
)

add_dependencies(tfm_test_spe TF-M)

install(DIRECTORY ${CMAKE_BINARY_DIR}/build-spe/bin DESTINATION ${CMAKE_BINARY_DIR})
