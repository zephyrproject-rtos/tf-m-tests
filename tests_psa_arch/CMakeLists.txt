#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

# CMake 3.21 and above requests projects to specify cpu/arch compile and link flags explicitly in
# Armclang. Link: https://cmake.org/cmake/help/latest/policy/CMP0123.html
# It is aligned with current Armclang toolchain implementation.
# Explictly set this policy to NEW behavior to eliminate long warnings. It shall be set in root
# CMakeLists.txt otherwise project() will throw out the warnings.
if(POLICY CMP0123)
    cmake_policy(SET CMP0123 NEW)
endif()

#--- SPE artifacts -------------------------------------------------------------

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR "CONFIG_SPE_PATH = ${CONFIG_SPE_PATH} is not defined or incorrect. Please provide full path to TF-M build artifacts using -DCONFIG_SPE_PATH=")
endif()

list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)

# A platform sprecific MCPU and architecture flags for NS side
include(${CONFIG_SPE_PATH}/platform/cpuarch.cmake)

include(spe_config)
include(${CONFIG_SPE_PATH}/platform/config.cmake)

set(CPU_ARCH ${PSA_API_TEST_CPU_ARCH})
set(SUITE    ${TEST_PSA_API})

if (NOT "${TEST_PSA_API}" STREQUAL "IPC")
    set(TARGET tgt_dev_apis_tfm_${PSA_API_TEST_TARGET})
else()
    set(TARGET tgt_ff_tfm_${PSA_API_TEST_TARGET})
endif()

#--- NSPE side project ---------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
include(toolchain_selection)

include(${TFM_TOOLCHAIN_FILE})
project("TF-M PSA Arch tests" LANGUAGES C)
tfm_toolchain_reload_compiler()

add_executable(tfm_ns)

add_subdirectory(../app_broker ${CMAKE_BINARY_DIR}/app_broker)

list(APPEND PSA_INCLUDE_PATHS ${CONFIG_SPE_PATH}/interface/include)

if(NOT PSA_INCLUDE_PATHS)
    set(PSA_INCLUDE_PATHS ${INTERFACE_INC_DIR}/
                            ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/
                            ${CMAKE_BINARY_DIR}/generated/interface/include
    )
endif()

set(PLATFORM_PSA_ISOLATION_LEVEL ${TFM_ISOLATION_LEVEL})

add_subdirectory(${PSA_ARCH_TESTS_PATH}/api-tests ${CMAKE_BINARY_DIR}/api-tests)

############################# TFM NS main app ##################################

target_sources(tfm_ns
    PRIVATE
        test_app.c
)

target_link_libraries(tfm_ns
    PRIVATE
        tfm_test_broker
        val_nspe
        pal_nspe
        test_combine
)

set_target_properties(tfm_ns PROPERTIES
    SUFFIX ".axf"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_add_scatter_file(tfm_ns ${CONFIG_SPE_PATH}/platform/linker_scripts)

target_link_options(tfm_ns
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/tfm_ns.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
)

add_convert_to_bin_target(tfm_ns)
