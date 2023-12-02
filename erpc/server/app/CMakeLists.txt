#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0123)
    cmake_policy(SET CMP0123 NEW)
endif()

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR "CONFIG_SPE_PATH = ${CONFIG_SPE_PATH} is not defined or incorrect. Please provide full path to TF-M build artifacts using -DCONFIG_SPE_PATH=")
endif()

list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../../cmake)

include(remote_library)
include(toolchain_selection)

# A platform specific MCPU and architecture flags for NS side
include(${CONFIG_SPE_PATH}/platform/cpuarch.cmake)
# Include common configs exported from TF-M
include(${CONFIG_SPE_PATH}/cmake/spe_config.cmake)

include(${TFM_TOOLCHAIN_FILE})

project("TF-M RPC Server (NSPE)"  LANGUAGES C ASM CXX)
tfm_toolchain_reload_compiler()

add_subdirectory(../../../lib/ext/CMSIS ${CMAKE_BINARY_DIR}/lib/ext/CMSIS)
add_subdirectory(../../../lib/ext/erpc ${CMAKE_BINARY_DIR}/lib/ext/erpc)

set(CONFIG_ENABLE_NS_UART_TX_RX_CONTROL ON)
set(ERPC_DIR     ${CMAKE_CURRENT_LIST_DIR}/../..)

add_subdirectory(${ERPC_DIR}/server ${CMAKE_CURRENT_BINARY_DIR}/server)
add_subdirectory(../../../app_broker ${CMAKE_BINARY_DIR}/app_broker)

add_executable(tfm_ns
    erpc_app.c
    # GNU Arm compiler version greater equal than *11.3.Rel1*
    # has a linker issue that required system calls are missing,
    # such as _read and _write. Add stub functions of required
    # system calls to solve this issue.
    $<$<BOOL:${CONFIG_GNU_SYSCALL_STUB_ENABLED}>:../../../app_broker/syscalls_stub.c>
    ${ERPC_REPO_PATH}/erpc_c/setup/erpc_setup_uart_cmsis.cpp
    ${ERPC_REPO_PATH}/erpc_c/transports/erpc_uart_cmsis_transport.cpp
)

target_include_directories(tfm_ns
    PRIVATE
        ${ERPC_REPO_PATH}/erpc_c/transports
        ${ERPC_DIR}/platform/${TFM_PLATFORM}
)

target_link_libraries(tfm_ns
    PRIVATE
        tfm_test_broker
        erpc_server
        tfm_api_ns
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
