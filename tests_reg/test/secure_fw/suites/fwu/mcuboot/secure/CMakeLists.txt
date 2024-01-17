#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT TEST_S_FWU)
    return()
endif()

add_library(tfm_test_suite_fwu_s STATIC)

target_sources(tfm_test_suite_fwu_s
    PRIVATE
        ../fwu_tests_common.c
        psa_fwu_s_interface_testsuite.c
)

target_include_directories(tfm_test_suite_fwu_s
    PUBLIC
        .
)

target_link_libraries(tfm_test_suite_fwu_s
    PRIVATE
        tfm_test_framework_s
        platform_s
)

target_compile_definitions(tfm_test_suite_fwu_s
    PRIVATE
        $<$<BOOL:${TFM_FWU_TEST_WRITE_WITH_NULL}>:TFM_FWU_TEST_WRITE_WITH_NULL>
        $<$<BOOL:${TFM_FWU_TEST_REQUEST_REBOOT}>:TFM_FWU_TEST_REQUEST_REBOOT>
        $<$<BOOL:${TFM_FWU_TEST_QUERY_WITH_NULL}>:TFM_FWU_TEST_QUERY_WITH_NULL>
        $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
    INTERFACE
        TEST_S_FWU
)

target_link_libraries(tfm_s_tests
    INTERFACE
        tfm_test_suite_fwu_s
)
