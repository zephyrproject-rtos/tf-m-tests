#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TFM_PARTITION_ATTESTATION_TEST)
    return()
endif()

add_library(tfm_psa_rot_partition_attest_test_service STATIC)

target_sources(tfm_psa_rot_partition_attest_test_service
    PRIVATE
        ./tfm_attest_test_service.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_attest_test_service
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/test_services/tfm_attest_test_service/auto_generated/intermedia_tfm_attest_test_service.c>
)

target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/test_services/tfm_attest_test_service/auto_generated/load_info_tfm_attest_test_service.c>
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_attest_test_service
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/test_services/tfm_attest_test_service
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/test_services/tfm_attest_test_service
)

target_link_libraries(tfm_psa_rot_partition_attest_test_service
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_psa_rot_partition_attest_test_service
)


# header files included by other libraries
add_library(tfm_psa_rot_partition_attest_test_service_defs INTERFACE)

target_include_directories(tfm_psa_rot_partition_attest_test_service_defs
    INTERFACE
        .
)

############################ NS API ############################################

if(TEST_FRAMEWORK_NS)
    target_sources(tfm_api_ns
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/tfm_attest_test_service_ns_api.c
    )

    # The veneers give warnings about not being properly declared so they get hidden
    # to not overshadow _real_ warnings.
    set_source_files_properties(tfm_attest_test_service_ns_api.c
        PROPERTIES
            COMPILE_FLAGS -Wno-implicit-function-declaration
    )

    target_include_directories(tfm_api_ns
        PUBLIC
            .
    )
endif()

# Export test service non-secure interface API
install(FILES       ${CMAKE_CURRENT_SOURCE_DIR}/tfm_attest_test_service_ns_api.c
        DESTINATION ${TEST_SERVICE_SRC_INSTALL_DIR})
install(FILES       ${CMAKE_CURRENT_SOURCE_DIR}/tfm_attest_test_service_api.h
        DESTINATION ${TEST_SERVICE_INC_INSTALL_DIR})

############################ Secure API ########################################

target_sources(tfm_secure_api
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_attest_test_service_secure_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_attest_test_service_secure_api.c
    PROPERTIES
        COMPILE_FLAGS -Wno-implicit-function-declaration
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_attest_test_service
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        ATTEST_TEST_GET_PUBLIC_KEY
)
