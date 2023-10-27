#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

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
    set(PSA_ARCH_TESTS_PATH     "DOWNLOAD"  CACHE PATH      "Path to PSA arch test repository used for SPE build")
    set(PSA_ARCH_TESTS_VERSION  "5c57920"   CACHE STRING    "The version of PSA arch tests to use")
    fetch_remote_library(
        LIB_NAME                psa_arch_tests
        LIB_SOURCE_PATH_VAR     PSA_ARCH_TESTS_PATH
        LIB_PATCH_DIR           ${CMAKE_CURRENT_SOURCE_DIR}
        LIB_FORCE_PATCH         PSA_ARCH_TESTS_FORCE_PATCH
        FETCH_CONTENT_ARGS
            GIT_REPOSITORY      https://github.com/ARM-software/psa-arch-tests.git
            GIT_TAG             ${PSA_ARCH_TESTS_VERSION}
            GIT_SHALLOW         FALSE
            GIT_PROGRESS        TRUE
            GIT_SUBMODULES      ""
    )
endif()

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CROSS_COMPILE         arm-none-eabi CACHE STRING "Cross-compiler prefix")
    set(CMAKE_TOOLCHAIN_FILE  ${CONFIG_TFM_SOURCE_PATH}/toolchain_GNUARM.cmake)
endif()

configure_file(${CMAKE_SOURCE_DIR}/tfm_psa_ff_test_manifest_list.yaml
               ${CMAKE_BINARY_DIR}/tfm_psa_ff_test_manifest_list.yaml)
list(APPEND TFM_EXTRA_MANIFEST_LIST_FILES ${CMAKE_BINARY_DIR}/tfm_psa_ff_test_manifest_list.yaml)

#------------------------------------------------------------------------------

# tfm_s_test is IMPORTED to inform CMake that it has no source files.
add_executable(tfm_test_spe IMPORTED)

include(ExternalProject)
ExternalProject_Add(TF-M
    SOURCE_DIR        ${CONFIG_TFM_SOURCE_PATH}
    BINARY_DIR        build-spe
    INSTALL_DIR       api_ns
    CMAKE_ARGS        -DTFM_PLATFORM=${TFM_PLATFORM}
    CMAKE_ARGS        -DTEST_PSA_API=${TEST_PSA_API}
    CMAKE_ARGS        -DTFM_EXTRA_CONFIG_PATH=${CMAKE_SOURCE_DIR}/config_spe.cmake
    CMAKE_ARGS        -DPROJECT_CONFIG_HEADER_FILE=${CMAKE_SOURCE_DIR}/config_test_psa_api.h
    CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    CMAKE_ARGS        -DTFM_EXTRA_MANIFEST_LIST_FILES=${TFM_EXTRA_MANIFEST_LIST_FILES}
    CMAKE_ARGS        -DPSA_ARCH_TESTS_PATH=${PSA_ARCH_TESTS_PATH}
    CMAKE_ARGS        -DCONFIG_TFM_TEST_DIR=${CMAKE_SOURCE_DIR}/partitions
    PREFIX             "temp"
)

add_dependencies(tfm_test_spe TF-M)

install(DIRECTORY ${CMAKE_BINARY_DIR}/build-spe/bin DESTINATION ${CMAKE_BINARY_DIR})
