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

if (NOT DEFINED TFM_TOOLCHAIN_FILE)
    set(TFM_TOOLCHAIN_FILE    ${CONFIG_SPE_PATH}/cmake/toolchain_ns_GNUARM.cmake)
endif()

# A platform sprecific MCPU and architecture flags for NS side
include(${CONFIG_SPE_PATH}/platform/cpuarch.cmake)
# Platform abilities for example IRQ test support status
include(${CONFIG_SPE_PATH}/platform/config.cmake OPTIONAL)

set(TFM_NS_REG_TEST       ON)
set(NS                    ON)

# Test suite configurations - set up by SPE build
include(${CONFIG_SPE_PATH}/config_ns_test.cmake)

# Test configurations
include(${CMAKE_CURRENT_LIST_DIR}/test/config/default_test_config.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)
include(remote_library)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib/ext ${CMAKE_BINARY_DIR}/lib/ext)

include(${TFM_TOOLCHAIN_FILE})
project(tfm_ns LANGUAGES C)
tfm_toolchain_reload_compiler()

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

target_add_scatter_file(tfm_ns ${CONFIG_SPE_PATH}/platform/linker_scripts)

target_link_options(tfm_ns
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/tfm_ns.map>
)

add_convert_to_bin_target(tfm_ns)
