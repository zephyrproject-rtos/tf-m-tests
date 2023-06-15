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

if (NOT DEFINED TFM_TEST_PATH)
    set(TFM_TEST_PATH ${CMAKE_SOURCE_DIR}/../test)
endif()

set(TFM_NS_REG_TEST       ON)
set(NS                    ON)
set(TEST_NS               ON)
#------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)

project(tfm_ns LANGUAGES C)

add_executable(tfm_ns)

add_subdirectory(../ns_interface ${CMAKE_BINARY_DIR}/ns_interface)
add_subdirectory(../test         ${CMAKE_BINARY_DIR}/tests)

include(../test/config/set_config.cmake)

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
