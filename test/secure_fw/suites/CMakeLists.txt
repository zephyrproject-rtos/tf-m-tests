#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

add_subdirectory(spm/ipc/service)
add_subdirectory(spm/sfn/service)
add_subdirectory(spm/irq/service)
add_subdirectory(ps/service)
add_subdirectory(fpu/service)

if(TFM_NS_REG_TEST AND NS)

    target_link_libraries(tfm_test_framework_common
        PUBLIC
            tfm_log
    )

    add_library(tfm_ns_tests INTERFACE)
    add_library(tfm_test_framework_ns INTERFACE)

    target_compile_definitions(tfm_test_framework_ns
        INTERFACE
            DOMAIN_NS=1
    )

    target_link_libraries(tfm_test_framework_ns
        INTERFACE
            tfm_test_framework_common
            tfm_api_ns
            tfm_log
    )

    target_sources(tfm_ns_tests
        INTERFACE
            ../non_secure_suites.c
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_framework_ns
            tfm_config
    )

    target_compile_definitions(tfm_ns_tests
        INTERFACE
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
            $<$<BOOL:${TFM_FUZZER_TOOL_TESTS}>:TFM_FUZZER_TOOL_TESTS>
    )
endif()

# Add test suites.
# Secure test suite library targets shall already be added in secure_tests.cmake
add_subdirectory(attestation)
add_subdirectory(crypto)
add_subdirectory(extra)
add_subdirectory(its)
add_subdirectory(ps)
# Do we need to test these external components?
# add_subdirectory(qcbor)
# add_subdirectory(t_cose)
add_subdirectory(platform)
add_subdirectory(fwu)
add_subdirectory(multi_core/non_secure)
add_subdirectory(nsid)
add_subdirectory(fpu)
add_subdirectory(spm)
