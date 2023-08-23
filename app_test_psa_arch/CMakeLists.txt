#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

#--- SPE artifacts -------------------------------------------------------------

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR "CONFIG_SPE_PATH = ${CONFIG_SPE_PATH} is not defined or incorrect. Please provide full path to TF-M build artifacts using -DCONFIG_SPE_PATH=")
endif()

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CROSS_COMPILE         arm-none-eabi)
    set(CMAKE_TOOLCHAIN_FILE  ${CONFIG_SPE_PATH}/cmake/toolchain_ns_GNUARM.cmake)
endif()

set(CPU_ARCH ${PSA_API_TEST_CPU_ARCH})
set(SUITE    ${TEST_PSA_API})

#--- NSPE side project ---------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)

project("TF-M PSA Arch tests" LANGUAGES C)

add_executable(tfm_ns)

add_subdirectory(../ns_interface ${CMAKE_BINARY_DIR}/ns_interface)

list(APPEND PSA_INCLUDE_PATHS ${CONFIG_SPE_PATH}/interface/include)

if(NOT TARGET)
    if (NOT "${TEST_PSA_API}" STREQUAL "IPC")
        set(TARGET tgt_dev_apis_tfm_${PSA_API_TEST_TARGET})
    else()
        set(TARGET tgt_ff_tfm_${PSA_API_TEST_TARGET})
    endif()
endif()

if(NOT PSA_INCLUDE_PATHS)
    set(PSA_INCLUDE_PATHS ${INTERFACE_INC_DIR}/
                            ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/
                            ${CMAKE_BINARY_DIR}/generated/interface/include
    )
endif()

set(SP_HEAP_MEM_SUPP 0)
set(PLATFORM_PSA_ISOLATION_LEVEL ${TFM_ISOLATION_LEVEL})

add_subdirectory(${PSA_ARCH_TESTS_PATH}/api-tests ${CMAKE_CURRENT_BINARY_DIR}/psa_api_tests)

if (TEST_PSA_API STREQUAL IPC)
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
endif()

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

