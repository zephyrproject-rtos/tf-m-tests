#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../utils)
include(utils)
include(regression_flag_parse)

collect_build_cmd_args(TFM_CMDLINE_CONFIGS)

project("TF-M SPE for tests" NONE)

if (NOT DEFINED CONFIG_TFM_SOURCE_PATH OR NOT EXISTS ${CONFIG_TFM_SOURCE_PATH})
    message(FATAL_ERROR "CONFIG_TFM_SOURCE_PATH = ${CONFIG_TFM_SOURCE_PATH} is not defined or incorrect. Please provide full path to TF-M sources.")
endif()

# Parse regression test flags, include both NS and S ones
parse_regression_flag(TFM_CMDLINE_CONFIGS)

# tfm_s_test is IMPORTED to inform CMake that it has no source files.
add_executable(tfm_s_test IMPORTED)

if(TFM_NS_REG_TEST OR TFM_S_REG_TEST OR TEST_BL2 OR TEST_BL1_1 OR TEST_BL1_2)
    set(CONFIG_TFM_TEST_DIR         "${CMAKE_CURRENT_LIST_DIR}/../test/secure_regression")
    set(CONFIG_TFM_TEST_CONFIG_FILE "${CMAKE_CURRENT_LIST_DIR}/../test/config/config.cmake")
endif()

include(ExternalProject)

ExternalProject_Add(TF-M
  SOURCE_DIR        ${CONFIG_TFM_SOURCE_PATH}
  BINARY_DIR        build-spe
  INSTALL_DIR       api_ns
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  CMAKE_ARGS        -DCONFIG_TFM_TEST_DIR=${CONFIG_TFM_TEST_DIR}
  CMAKE_ARGS        -DCONFIG_TFM_TEST_CONFIG_FILE=${CONFIG_TFM_TEST_CONFIG_FILE}
  CMAKE_CACHE_DEFAULT_ARGS ${TFM_CMDLINE_CONFIGS}
  PREFIX             "temp"
  BUILD_ALWAYS       ON
)

add_dependencies(tfm_s_test TF-M)

install(DIRECTORY ${CMAKE_BINARY_DIR}/build-spe/bin DESTINATION ${CMAKE_BINARY_DIR})

install(DIRECTORY   ${CONFIG_TFM_SOURCE_PATH}/secure_fw/partitions/initial_attestation
        DESTINATION ${CMAKE_BINARY_DIR}/api_ns
        FILES_MATCHING PATTERN "*.h")

install(FILES       ${CONFIG_TFM_SOURCE_PATH}/secure_fw/spm/include/boot/tfm_boot_status.h
        DESTINATION ${CMAKE_BINARY_DIR}/api_ns/initial_attestation)

install(DIRECTORY   ${CONFIG_TFM_SOURCE_PATH}/lib/ext/t_cose
        DESTINATION ${CMAKE_BINARY_DIR}/api_ns)

install(FILES       ${CONFIG_TFM_SOURCE_PATH}/lib/ext/qcbor/q_useful_buf.h
        DESTINATION ${CMAKE_BINARY_DIR}/api_ns/qcbor/inc)
