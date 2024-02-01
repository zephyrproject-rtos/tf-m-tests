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

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
include(toolchain_selection)

#--- SPE artifacts -------------------------------------------------------------

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR "CONFIG_SPE_PATH = ${CONFIG_SPE_PATH} is not defined or incorrect. Please provide full path to TF-M build artifacts using -DCONFIG_SPE_PATH=")
endif()

list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)

# A platform sprecific MCPU and architecture flags for NS side
include(${CONFIG_SPE_PATH}/platform/cpuarch.cmake)
# Configs exported by TF-M build
include(spe_config)
include(${CONFIG_SPE_PATH}/platform/config.cmake OPTIONAL)
# Include platform specific PSA Arch test preferences
include(${CONFIG_SPE_PATH}/platform/tests/psa_arch_tests_config.cmake OPTIONAL)

#--- NSPE side project ---------------------------------------------------------

include(${TFM_TOOLCHAIN_FILE})
project("TF-M PSA Arch tests" LANGUAGES C ASM)
tfm_toolchain_reload_compiler()

# Setup required configs: SUITE, CPU_ARCH and TARGET
set(SUITE    ${TEST_PSA_API})

if (NOT DEFINED CPU_ARCH)
    if (${TFM_SYSTEM_ARCHITECTURE} STREQUAL armv8-m.main)
        set(CPU_ARCH armv8m_ml)
    elseif (${TFM_SYSTEM_ARCHITECTURE} STREQUAL armv8-m.base)
        set(CPU_ARCH armv8m_bl)
    elseif (${TFM_SYSTEM_ARCHITECTURE} STREQUAL armv8.1-m.main)
        set(CPU_ARCH armv81m_ml)
    elseif (${TFM_SYSTEM_ARCHITECTURE} STREQUAL armv7-m)
        set(CPU_ARCH armv7m)
    endif()
endif()

if (NOT DEFINED PSA_API_TEST_TARGET)
    string(REGEX REPLACE ".*/" "" PSA_API_TEST_TARGET ${TFM_PLATFORM})
endif()

if(NOT SP_HEAP_MEM_SUPP)
    set(SP_HEAP_MEM_SUPP 0)
endif()

if ("${TEST_PSA_API}" STREQUAL "IPC")
    set(TARGET tgt_ff_tfm_${PSA_API_TEST_TARGET})
else()
    set(TARGET tgt_dev_apis_tfm_${PSA_API_TEST_TARGET})
endif()

# Toolchain
if (NOT TOOLCHAIN)
    if (${CMAKE_C_COMPILER_ID} STREQUAL GNU)
        set(TOOLCHAIN GNUARM)
    elseif (${CMAKE_C_COMPILER_ID} STREQUAL ARMClang)
        set(TOOLCHAIN ARMCLANG)
    endif()
endif()

# Check config in case additional configs are passed via command line
include(${CMAKE_SOURCE_DIR}/spe/config/check_config.cmake)

add_executable(tfm_ns)

add_subdirectory(../lib/ext ${CMAKE_BINARY_DIR}/lib/ext)
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

if ("${TEST_PSA_API}" STREQUAL "CRYPTO")
    # psa_crypto_config needs to have visibility of the platform includes
    target_link_libraries(psa_crypto_config INTERFACE platform_ns)
    # val_nspe/pal_nspe targets are calling PSA Crypto APIs so they need
    # to include the psa_crypto_config which exports the config file defs
    target_link_libraries(val_nspe PUBLIC psa_crypto_config)
    target_link_libraries(pal_nspe PUBLIC psa_crypto_config)
endif()

############################# TFM NS main app ##################################

target_sources(tfm_ns
    PRIVATE
        test_app.c
        $<$<BOOL:${CONFIG_GNU_SYSCALL_STUB_ENABLED}>:../app_broker/syscalls_stub.c>
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
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/tfm_ns.map>
)

add_convert_to_bin_target(tfm_ns)
