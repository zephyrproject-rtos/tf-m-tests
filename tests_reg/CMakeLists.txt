#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR "CONFIG_SPE_PATH = ${CONFIG_SPE_PATH} is not defined or incorrect. Please provide full path to TF-M build artifacts using -DCONFIG_SPE_PATH=")
endif()

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CROSS_COMPILE         arm-none-eabi)
    set(CMAKE_TOOLCHAIN_FILE  ${CONFIG_SPE_PATH}/cmake/toolchain_ns_GNUARM.cmake)
endif()

#--- Test configuration --------------------------------------------------------
# include(../test/config/default_ns_test_config.cmake)
# Temporary hardcoded. Need better mechnism of NS tests selectoin

set(TFM_NS_REG_TEST       ON)
set(NS                    ON)
set(TEST_NS               ON)

set(TEST_NS_ATTESTATION     OFF        CACHE BOOL      "Whether to build NS regression Attestation tests")
set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
set(TEST_NS_CRYPTO          ON        CACHE BOOL      "Whether to build NS regression Crypto tests")
set(TEST_NS_ITS             ON        CACHE BOOL      "Whether to build NS regression ITS tests")
set(TEST_NS_PS              ON        CACHE BOOL      "Whether to build NS regression PS tests")
set(TEST_NS_PLATFORM        ON        CACHE BOOL      "Whether to build NS regression Platform tests")
set(TEST_NS_FWU             OFF        CACHE BOOL      "Whether to build NS regression FWU tests")
set(TEST_NS_MULTI_CORE      OFF        CACHE BOOL      "Whether to build NS regression multi-core tests")
set(TEST_NS_MANAGE_NSID     OFF        CACHE BOOL      "Whether to build NS regression NSID management tests")
#------------------------------------------------------------------------------

# Include default test configurations (this not about which test suites to enable or disable)
include(${CMAKE_CURRENT_LIST_DIR}/test/config/default_test_config.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)

project(tfm_ns LANGUAGES C)

add_executable(tfm_ns)

add_subdirectory(../app_broker ${CMAKE_BINARY_DIR}/app_broker)
add_subdirectory(test)

############################# TFM NS main app ##################################

target_sources(tfm_ns
    PRIVATE
        test_app.c
)

target_link_libraries(tfm_ns
    PRIVATE
        tfm_test_broker
        tfm_ns_tests
        tfm_test_framework_common
)

set_target_properties(tfm_ns PROPERTIES
    SUFFIX ".axf"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
