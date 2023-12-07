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

if (NOT DEFINED CONFIG_SPE_PATH OR NOT EXISTS ${CONFIG_SPE_PATH})
    message(FATAL_ERROR "CONFIG_SPE_PATH = ${CONFIG_SPE_PATH} is not defined or incorrect. Please provide full path to TF-M build artifacts using -DCONFIG_SPE_PATH=")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
list(APPEND CMAKE_MODULE_PATH ${CONFIG_SPE_PATH}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils)
include(remote_library)
include(toolchain_selection)
include(utils)

# A platform sprecific MCPU and architecture flags for NS side
include(${CONFIG_SPE_PATH}/platform/cpuarch.cmake)
# Include common configs exported from TF-M
include(${CONFIG_SPE_PATH}/cmake/spe_config.cmake)
# Platform abilities for example IRQ test support status
include(${CONFIG_SPE_PATH}/platform/config.cmake OPTIONAL)

include(${TFM_TOOLCHAIN_FILE})
project(tfm_ns LANGUAGES C ASM)
tfm_toolchain_reload_compiler()

# If config_ns_test.cmake is exported from TF-M build, it indicates regression tests are enabled.
# Otherwise, skip test configuration
include(${CONFIG_SPE_PATH}/config/config_ns_test.cmake OPTIONAL)
if(TFM_NS_REG_TEST)
    # Include platform specific regression tests config
    include(${CONFIG_SPE_PATH}/platform/tests/tfm_tests_config.cmake OPTIONAL)
    if (TFM_PROFILE)
        include(${CMAKE_CURRENT_LIST_DIR}/test/config/profile/${TFM_PROFILE}_test.cmake)
    endif()
    # Default test configurations
    include(${CMAKE_CURRENT_LIST_DIR}/test/config/default_test_config.cmake)
    # Config check in case additional test configs passed in via command line.
    include(${CMAKE_CURRENT_LIST_DIR}/test/config/check_config.cmake)
endif()
# Include coprocessor check to make sure NS toolchain is compatiable
include(${CONFIG_SPE_PATH}/config/cp_check.cmake)

add_executable(tfm_ns)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib/ext ${CMAKE_BINARY_DIR}/lib/ext)
add_subdirectory(../app_broker ${CMAKE_BINARY_DIR}/app_broker)
if(TFM_NS_REG_TEST)
    add_subdirectory(test/ns_regression)
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
        $<$<BOOL:${TFM_NS_REG_TEST}>:tfm_ns_tests>
)

target_compile_definitions(tfm_ns
    PUBLIC
        $<$<BOOL:${TFM_NS_REG_TEST}>:TFM_NS_REG_TEST>
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
