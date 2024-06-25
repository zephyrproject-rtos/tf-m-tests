#-------------------------------------------------------------------------------
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR "CONFIG_SPE_PATH = ${CONFIG_SPE_PATH} is not defined or incorrect. Please provide full path to TF-M build artifacts using -DCONFIG_SPE_PATH=")
endif()

if (ERPC_TRANSPORT STREQUAL "UART")
    if (NOT DEFINED PORT_NAME)
        message(FATAL_ERROR "Please provide PORT_NAME!")
    endif()
elseif (ERPC_TRANSPORT STREQUAL "TCP")
    if((NOT DEFINED ERPC_HOST) OR (NOT DEFINED ERPC_PORT))
        message(FATAL_ERROR "Please provide ERPC_HOST and ERPC_PORT!")
    endif()
elseif (DEFINED ERPC_TRANSPORT)
    message(FATAL_ERROR "Please provide supported transportation type (UART or TCP)!")
endif()

if (NOT EXISTS ${ERPC_REPO_PATH})
    message(FATAL_ERROR "ERPC_REPO_PATH=${ERPC_REPO_PATH} is NOT a valid path!")
endif()

list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)

include(utils)
include(remote_library)

project("ERPC Host" LANGUAGES CXX C)

add_executable(erpc_main)

# eRPC client
add_subdirectory(../client client)

target_sources(erpc_main
    PRIVATE
        main_host.c
        $<$<STREQUAL:${ERPC_TRANSPORT},UART>:${ERPC_REPO_PATH}/erpc_c/setup/erpc_setup_serial.cpp>
        $<$<STREQUAL:${ERPC_TRANSPORT},UART>:${ERPC_REPO_PATH}/erpc_c/transports/erpc_serial_transport.cpp>

        $<$<STREQUAL:${ERPC_TRANSPORT},TCP>:${ERPC_REPO_PATH}/erpc_c/setup/erpc_setup_tcp.cpp>
        $<$<STREQUAL:${ERPC_TRANSPORT},TCP>:${ERPC_REPO_PATH}/erpc_c/transports/erpc_tcp_transport.cpp>
        $<$<STREQUAL:${ERPC_TRANSPORT},TCP>:${ERPC_REPO_PATH}/erpc_c/port/erpc_threading_pthreads.cpp>
)

target_compile_definitions(erpc_main
    PRIVATE
        $<$<STREQUAL:${ERPC_TRANSPORT},UART>:ERPC_TRANSPORT_UART>
        $<$<AND:$<STREQUAL:${ERPC_TRANSPORT},UART>,$<BOOL:${PORT_NAME}>>:PORT_NAME="${PORT_NAME}">

        $<$<STREQUAL:${ERPC_TRANSPORT},TCP>:ERPC_TRANSPORT_TCP>
        $<$<AND:$<STREQUAL:${ERPC_TRANSPORT},TCP>,$<BOOL:${ERPC_HOST}>>:ERPC_HOST="${ERPC_HOST}">
        $<$<AND:$<STREQUAL:${ERPC_TRANSPORT},TCP>,$<BOOL:${ERPC_PORT}>>:ERPC_PORT=${ERPC_PORT}>
)

################################ SPE interfaces ################################
# Configs exported by TF-M build
include(spe_config)
# Include exported targets as we need psa_crypto_config/psa_interface
include(spe_export)

set_target_properties(tfm_config psa_interface PROPERTIES IMPORTED_GLOBAL True)
target_link_libraries(tfm_config INTERFACE psa_interface)

# Check config
if (TFM_NS_MANAGE_NSID)
    message(FATAL_ERROR "The RPC test framework does not support NS ID management (TFM_NS_MANAGE_NSID)!")
endif()

# In actual NS integration, NS side build should include the source files
# exported by TF-M build.
set(INTERFACE_SRC_DIR    ${CONFIG_SPE_PATH}/interface/src)
set(INTERFACE_INC_DIR    ${CONFIG_SPE_PATH}/interface/include)
set(PLATFORM_INC_DIR     ${CONFIG_SPE_PATH}/platform/include)   # For crypto accelerator configuration

# INTERFACE because there might be no API files below
add_library(tfm_api_ns INTERFACE)

target_sources(tfm_api_ns
    INTERFACE
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${INTERFACE_SRC_DIR}/tfm_platform_api.c>
        $<$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>:${INTERFACE_SRC_DIR}/tfm_ps_api.c>
        $<$<BOOL:${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}>:${INTERFACE_SRC_DIR}/tfm_its_api.c>
        $<$<BOOL:${TFM_PARTITION_CRYPTO}>:${INTERFACE_SRC_DIR}/tfm_crypto_api.c>
        $<$<BOOL:${TFM_PARTITION_INITIAL_ATTESTATION}>:${INTERFACE_SRC_DIR}/tfm_attest_api.c>
        $<$<BOOL:${TFM_PARTITION_FIRMWARE_UPDATE}>:${INTERFACE_SRC_DIR}/tfm_fwu_api.c>
)

# Include interface headers exported by TF-M
target_include_directories(tfm_api_ns
    INTERFACE
        ${INTERFACE_INC_DIR}
        ${PLATFORM_INC_DIR}
        $<$<BOOL:${PLATFORM_DEFAULT_CRYPTO_KEYS}>:${INTERFACE_INC_DIR}/crypto_keys>
)

if (TFM_PARTITION_CRYPTO)
target_link_libraries(tfm_api_ns
    INTERFACE
        psa_crypto_config
)
endif()

add_library(platform_region_defs INTERFACE)

target_compile_definitions(platform_region_defs
    INTERFACE
        $<$<BOOL:${BL1}>:BL1>
        $<$<BOOL:${BL2}>:BL2>
        BL2_HEADER_SIZE=${BL2_HEADER_SIZE}
        BL2_TRAILER_SIZE=${BL2_TRAILER_SIZE}
        BL1_HEADER_SIZE=${BL1_HEADER_SIZE}
        BL1_TRAILER_SIZE=${BL1_TRAILER_SIZE}
        $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:ENABLE_HEAP>
)

target_include_directories(platform_region_defs
    INTERFACE
        ${CONFIG_SPE_PATH}/platform/partition
)

################################# Build Tests ##################################
# If config_ns_test.cmake is exported from TF-M build, it indicates regression
# tests are enabled. Otherwise, skip test configuration.
set(TFM_REG_TEST_ROOT   ../../tests_reg/test)
include(${CONFIG_SPE_PATH}/config/config_ns_test.cmake OPTIONAL)

set(CONFIG_TFM_ERPC_TEST_FRAMEWORK  ON)
# Disable Non-TF-M tests
set(TEST_NS_QCBOR   OFF CACHE BOOL  "Whether to build NS regression qcbor tests" FORCE)
set(TEST_NS_T_COSE  OFF CACHE BOOL  "Whether to build NS regression t_cose tests" FORCE)

if (NOT TFM_NS_REG_TEST)
    message(FATAL_ERROR "TFM_NS_REG_TEST is NOT enabled!")
endif()

# Include platform specific regression tests config
include(${CONFIG_SPE_PATH}/platform/tests/tfm_tests_config.cmake OPTIONAL)
if (TFM_PROFILE)
    include(${TFM_REG_TEST_ROOT}/config/profile/${TFM_PROFILE}_test.cmake)
endif()
# Default test configurations
include(${TFM_REG_TEST_ROOT}/config/default_test_config.cmake)
# Config check in case additional test configs passed in via command line.
include(${TFM_REG_TEST_ROOT}/config/check_config.cmake)

# Dummy tfm_ns_log for the tfm_test_framework_ns library
add_library(tfm_ns_log INTERFACE)

set(USE_STDIO ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../lib/ext/qcbor ${CMAKE_BINARY_DIR}/lib/ext/qcbor)
add_subdirectory(${TFM_REG_TEST_ROOT}/ns_regression      ${CMAKE_BINARY_DIR}/ns_regression)

target_link_libraries(erpc_main
    PRIVATE
        erpc_client
        tfm_api_ns
        tfm_ns_tests
        $<$<STREQUAL:${ERPC_TRANSPORT},TCP>:pthread>
)
