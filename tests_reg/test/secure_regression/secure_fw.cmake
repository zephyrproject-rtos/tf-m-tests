#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(utils)
    dump_options("TEST Configuration"
    "
        TEST_NS;
        TEST_S;
        TEST_NS_ATTESTATION;
        TEST_NS_CRYPTO;
        TEST_NS_ITS;
        TEST_NS_PS;
        TEST_NS_QCBOR;
        TEST_NS_T_COSE;
        TEST_NS_PLATFORM;
        TEST_NS_FWU;
        TEST_NS_IPC;
        TEST_NS_SLIH_IRQ;
        TEST_NS_FLIH_IRQ;
        TEST_NS_MULTI_CORE;
        TEST_NS_MANAGE_NSID;
        TEST_NS_SFN_BACKEND;
        TEST_NS_FPU;
        TEST_S_ATTESTATION;
        TEST_S_CRYPTO;
        TEST_S_ITS;
        TEST_S_PS;
        TEST_S_PLATFORM;
        TEST_S_FWU;
        TEST_S_IPC;
        TEST_S_SFN_BACKEND;
        TEST_S_FPU;
    "
    )

add_library(tfm_test_framework_s INTERFACE)
add_library(tfm_s_tests STATIC)

target_link_libraries(tfm_test_framework_s
    INTERFACE
        psa_interface
        tfm_test_framework_common
        tfm_sprt
        tfm_config
)

target_compile_definitions(tfm_test_framework_s
    INTERFACE
        USE_SP_LOG
)

target_sources(tfm_s_tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/secure_suites.c
)

target_link_libraries(tfm_s_tests
    PUBLIC
        tfm_test_framework_s
        tfm_config
        tfm_spm
)

target_compile_definitions(tfm_s_tests
    PRIVATE
        $<$<BOOL:${PS_TEST_NV_COUNTERS}>:PS_TEST_NV_COUNTERS>
)

add_subdirectory(${SECURE_FW_REG_DIR}/secure
                 ${CMAKE_CURRENT_BINARY_DIR}/secure_fw
)
