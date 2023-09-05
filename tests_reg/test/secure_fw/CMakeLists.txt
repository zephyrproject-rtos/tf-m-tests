#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

########################## TEST Configuration ##################################

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

if (TFM_S_REG_TEST)

    # secure test services are required if any secure test is opened
    add_subdirectory(common_test_services/tfm_secure_client_service)
    add_subdirectory(common_test_services/tfm_secure_client_2)

    add_library(tfm_test_framework_s INTERFACE)

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
            $<$<BOOL:${PS_TEST_NV_COUNTERS}>:PS_TEST_NV_COUNTERS>
    )

    target_link_libraries(tfm_test_framework_common
        PUBLIC
            tfm_test_framework_s
    )

    add_library(tfm_s_tests STATIC)

    target_sources(tfm_s_tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/secure_suites.c
    )

    target_link_libraries(tfm_s_tests
        PUBLIC
            tfm_test_framework_s
            tfm_spm
    )

endif()

# Include test suites at last after other targets are setup.
add_subdirectory(suites)
