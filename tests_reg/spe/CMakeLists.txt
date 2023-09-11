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

project("TF-M SPE for tests" LANGUAGES C)

if (NOT DEFINED CONFIG_TFM_SOURCE_PATH OR NOT EXISTS ${CONFIG_TFM_SOURCE_PATH})
#TODO: Fetch TF-M repositry if not exists
    message(FATAL_ERROR "CONFIG_TFM_SOURCE_PATH = ${CONFIG_TFM_SOURCE_PATH} is not defined or incorrect. Please provide full path to TF-M sources.")
endif()

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CROSS_COMPILE         arm-none-eabi CACHE STRING "Cross-compiler prefix")
    set(CMAKE_TOOLCHAIN_FILE  ${CONFIG_TFM_SOURCE_PATH}/toolchain_GNUARM.cmake)
endif()

if (NOT DEFINED CONFIG_TFM_REG_TESTS OR NOT EXISTS ${CONFIG_TFM_REG_TESTS})
    message(WARNING "The default refgression test set is used. Use CONFIG_TFM_REG_TESTS argument to specify alternative configuration")
    set(CONFIG_TFM_REG_TESTS ${CMAKE_SOURCE_DIR}/config_test.cmake)
  endif()

#------------------------------------------------------------------------------

# tfm_s_test is IMPORTED to inform CMake that it has no source files.
add_executable(tfm_s_test IMPORTED)

include(ExternalProject)
ExternalProject_Add(TF-M
  SOURCE_DIR        ${CONFIG_TFM_SOURCE_PATH}
  BINARY_DIR        build-spe
  INSTALL_DIR       api_ns
  CMAKE_ARGS        -DTFM_PLATFORM=${TFM_PLATFORM} -DCONFIG_TFM_TEST_DIR=${CMAKE_SOURCE_DIR}/.. -DTFM_EXTRA_CONFIG_PATH=${CONFIG_TFM_REG_TESTS} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  PREFIX             "temp"
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
