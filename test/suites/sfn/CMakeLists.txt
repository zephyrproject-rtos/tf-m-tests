#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

####################### Non Secure #############################################

if (TEST_NS_SFN_BACKEND)
    add_library(tfm_test_suite_sfn_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_sfn_ns
        PRIVATE
            non_secure/sfn_backend_ns_testsuite.c
    )

    target_include_directories(tfm_test_suite_sfn_ns
        PUBLIC
            non_secure
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../../test_services/tfm_test_sfn_partitions
    )

    target_compile_definitions(tfm_test_suite_sfn_ns
        PRIVATE
            DOMAIN_NS=1
        INTERFACE
            TEST_NS_SFN_BACKEND
    )

    target_link_libraries(tfm_test_suite_sfn_ns
        PRIVATE
            tfm_test_framework_ns
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_sfn_ns
    )
endif()
