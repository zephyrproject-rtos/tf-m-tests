#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# MCUBoot does not have its own log system, use the NS log one.
# However the tfm_log library can not be used directly as it's intended for NS tests.
# It links platform_ns which is not built by SPE build.
# So a dedicated library is duplicated here, and it links the platform_s instead of platform_ns.

# TO-DO: find a better approach to reference the directory
set(LOG_SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../../../lib/log)

add_library(mcuboot_test_log INTERFACE)
add_library(mcuboot_test_log_interface INTERFACE)

target_sources(mcuboot_test_log
    INTERFACE
        ${LOG_SOURCE_ROOT}/tfm_log_raw.c
)

target_include_directories(mcuboot_test_log_interface
    INTERFACE
        ${LOG_SOURCE_ROOT}
)

target_link_libraries(mcuboot_test_log
    INTERFACE
        mcuboot_test_log_interface
        platform_common_interface
)

add_subdirectory(suites/integration)

add_library(mcuboot_tests STATIC)

target_sources(mcuboot_tests
    PRIVATE
        ./mcuboot_suites.c
)

target_include_directories(mcuboot_tests
    PUBLIC
        .
)

target_link_libraries(mcuboot_tests
    PUBLIC
        tfm_test_framework_common
    PRIVATE
        platform_bl2
        mcuboot_test_suite_integration
        mcuboot_test_log
)
