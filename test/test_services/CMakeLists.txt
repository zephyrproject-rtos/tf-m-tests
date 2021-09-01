#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Since the main test directory is a NS dir, this subdir is specifically made a
# secure directory as it build the secure side test services
if (TFM_MULTI_CORE_TOPOLOGY)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload.cmake)
    tfm_toolchain_reload_compiler()
endif()

if (TEST_FRAMEWORK_S)
    # Secure tests are declared in this dir so that it can use the secure CPU
    # definitions
    add_library(tfm_s_tests INTERFACE)
    if (TEST_S_CRYPTO)
        add_library(tfm_test_suite_crypto_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TEST_S_ATTESTATION)
        add_library(tfm_test_suite_attestation_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TEST_S_ITS)
        add_library(tfm_test_suite_its_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TEST_S_PS)
        add_library(tfm_test_suite_ps_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TEST_S_AUDIT)
        add_library(tfm_test_suite_audit_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TEST_S_PLATFORM)
        add_library(tfm_test_suite_platform_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TEST_S_IPC)
        add_library(tfm_test_suite_ipc_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TEST_S_FWU)
        add_library(tfm_test_suite_fwu_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (EXTRA_S_TEST_SUITES_PATHS)
        add_library(tfm_test_suite_extra_s STATIC EXCLUDE_FROM_ALL)
    endif()
endif()

if (TEST_NS_ATTESTATION OR TEST_S_ATTESTATION)
    add_subdirectory(tfm_attest_test_service)
endif()

if (TEST_NS_CORE)
    add_subdirectory(tfm_core_test)
    add_subdirectory(tfm_core_test_2)
endif()

if (TFM_PSA_API)
    add_subdirectory(tfm_ipc_client)
    add_subdirectory(tfm_ipc_service)
endif()

if (TEST_S_PS)
    add_subdirectory(tfm_ps_test_service)
endif()

if (TEST_FRAMEWORK_S)
    add_subdirectory(tfm_secure_client_service)
    add_subdirectory(tfm_secure_client_2)
endif()

if (TEST_NS_SLIH_IRQ)
    add_subdirectory(tfm_slih_test_service)
endif()

if (TEST_NS_FLIH_IRQ)
    add_subdirectory(tfm_flih_test_service)
endif()
