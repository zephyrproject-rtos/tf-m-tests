#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

# Check configuration.
# Config check shall be performed after all configurations complete.
# Difficult to insert a hook to perform test config check inside configuration sequence.
# Put the check here before test partition build starts or is skipped.
include(${CMAKE_CURRENT_LIST_DIR}/../config/check_config.cmake)

if (NOT "${TEST_PSA_API}" STREQUAL IPC)
    # The extra 3 TF-M partitions are necesary only for IPC tests
    return()
endif()

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

# Toolchain
if (NOT TOOLCHAIN)
    if (${CMAKE_C_COMPILER_ID} STREQUAL GNU)
        set(TOOLCHAIN GNUARM)
    elseif (${CMAKE_C_COMPILER_ID} STREQUAL ARMClang)
        set(TOOLCHAIN ARMCLANG)
    endif()
endif()

if (NOT DEFINED PSA_API_TEST_TARGET)
    string(REGEX REPLACE ".*/" "" PSA_API_TEST_TARGET ${TFM_PLATFORM})
endif()

set(SP_HEAP_MEM_SUPP 0)

set(TARGET tgt_ff_tfm_${PSA_API_TEST_TARGET})
set(SUITE    ${TEST_PSA_API})
set(PLATFORM_PSA_ISOLATION_LEVEL ${TFM_ISOLATION_LEVEL})

list(APPEND PSA_INCLUDE_PATHS ${CMAKE_SOURCE_DIR}/interface/include)
list(APPEND PSA_INCLUDE_PATHS ${CMAKE_BINARY_DIR}/generated/interface/include)
list(APPEND PSA_INCLUDE_PATHS ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests)

add_subdirectory(${PSA_ARCH_TESTS_PATH}/api-tests ${CMAKE_BINARY_DIR}/api-tests)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests
)

target_sources(tfm_psa_rot_partition_driver_partition
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/auto_generated/intermedia_driver_partition_psa.c
)
target_link_libraries(tfm_psa_rot_partition_driver_partition
    PRIVATE
        psa_interface
        platform_s
)
target_compile_definitions(tfm_psa_rot_partition_driver_partition
    PRIVATE
        CONFIG_TFM_BUILDING_SPE=1
        TFM_ISOLATION_LEVEL=${TFM_ISOLATION_LEVEL}
)

target_sources(tfm_app_rot_partition_client_partition
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/auto_generated/intermedia_client_partition_psa.c
)
target_link_libraries(tfm_app_rot_partition_client_partition
    PRIVATE
        psa_interface
        platform_s
)
target_compile_definitions(tfm_app_rot_partition_client_partition
    PRIVATE
        CONFIG_TFM_BUILDING_SPE=1
        TFM_ISOLATION_LEVEL=${TFM_ISOLATION_LEVEL}
)

target_sources(tfm_app_rot_partition_server_partition
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/auto_generated/intermedia_server_partition_psa.c
)
target_link_libraries(tfm_app_rot_partition_server_partition
    PRIVATE
        psa_interface
        platform_s
)
target_compile_definitions(tfm_app_rot_partition_server_partition
    PRIVATE
        CONFIG_TFM_BUILDING_SPE=1
        TFM_ISOLATION_LEVEL=${TFM_ISOLATION_LEVEL}
)

target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/auto_generated/load_info_driver_partition_psa.c
        ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/auto_generated/load_info_client_partition_psa.c
        ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/auto_generated/load_info_server_partition_psa.c
)

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_driver_partition
        tfm_app_rot_partition_client_partition
        tfm_app_rot_partition_server_partition
)

# Extra files required for IPC suit.

install(FILES       ${CMAKE_BINARY_DIR}/generated/interface/include/psa_manifest/pid.h
                    ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/psa_manifest/client_partition_psa.h
                    ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/psa_manifest/driver_partition_psa.h
                    ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/psa_manifest/server_partition_psa.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/interface/include/psa_manifest)

# TODO: Those fils should not be required by NSPE build.
install(FILES       ${CMAKE_SOURCE_DIR}/interface/include/psa/service.h
                    ${CMAKE_SOURCE_DIR}/interface/include/psa/lifecycle.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/interface/include/psa)
