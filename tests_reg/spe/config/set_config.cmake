#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Profile
if(TFM_PROFILE)
    include(${CONFIG_TFM_SOURCE_PATH}/config/profile/${TFM_PROFILE}.cmake)
    set(PROFILE_TEST_CONFIG_FILE    ${CMAKE_CURRENT_LIST_DIR}/../../test/config/profile/${TFM_PROFILE}_test.cmake)
else()
    set(PROFILE_TEST_CONFIG_FILE    ${CMAKE_CURRENT_LIST_DIR}/../../test/config/default_test_config.cmake)
endif()

# Backend config is required to enable or disable corresponding test suites
# Set up backend config
if((DEFINED TFM_ISOLATION_LEVEL AND TFM_ISOLATION_LEVEL GREATER 1) OR
    CONFIG_TFM_SPM_BACKEND STREQUAL "IPC" OR TFM_MULTI_CORE_TOPOLOGY)
    set(CONFIG_TFM_SPM_BACKEND_IPC  ON)
    set(CONFIG_TFM_SPM_BACKEND_SFN  OFF)
else()
    #The default backend is SFN
    set(CONFIG_TFM_SPM_BACKEND_IPC  OFF)
    set(CONFIG_TFM_SPM_BACKEND_SFN  ON)
endif()

# Get platform configurations, for example, PLATFORM_HAS_FIRMWARE_UPDATE_SUPPORT
include(${CONFIG_TFM_SOURCE_PATH}/config/tfm_platform.cmake)
include(${TARGET_PLATFORM_PATH}/config.cmake OPTIONAL)

# Set up test suites configurations according to command-line options.
# By default (no test suites configs at all), configs are set according to default_config.cmake.
# Users can:
# 1. Disable all the S/NS tests by setting corresponding TEST_S/TEST_NS to OFF
# 2. Disable individual test suites by setting the corresponding TEST_(N)S_* to OFF.
#    Other test suites are set according to default_config.cmake
# 3. Enable individual test suites by setting the corresponding TEST_(N)S_* to ON.
#    Other test suites are disabled by default. To enable other test suites, set the corresponding
#    config option to ON. This applies to TEST_S and TEST_NS as well which can be used to enable
#    test suites that are disabled by default even then TEST_NS/S is enabled.

# Check if user enabled individual test suites.
# If not, the default config is applied
set(LOAD_DEFAULT_CONFIG ON)
get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
    string(REGEX MATCH "^TEST_S.+" _S_TEST_FOUND "${CACHE_VAR}")
    if (_S_TEST_FOUND AND "${${CACHE_VAR}}")
        # User enabled individual S test suite, by default all other S test suites are off,
        # Unless TEST_S is explicity set to ON
        set(TEST_S OFF CACHE BOOL "Enable Secure Tests")
        set(LOAD_DEFAULT_CONFIG OFF)
        break()
    endif()
    string(REGEX MATCH "^TEST_NS.+" _NS_TEST_FOUND "${CACHE_VAR}")
    if (_NS_TEST_FOUND AND "${${CACHE_VAR}}")
        # User enabled individual NS test suite, by default all other NS test suites are OFF,
        # Unless TEST_NS is explicity set to ON
        set(TEST_NS OFF CACHE BOOL "Enable NS Tests")
        set(LOAD_DEFAULT_CONFIG OFF)
        break()
    endif()
endforeach()

# If the user also enable TEST_S/TEST_NS, the default configs should also be loaded.
if((TEST_S OR TEST_NS) AND NOT LOAD_DEFAULT_CONFIG)
    set(LOAD_DEFAULT_CONFIG ON)
endif()

if(LOAD_DEFAULT_CONFIG)
    # If profile is set, then test suites are enabled according to Secure Partitions.
    # On the contrary, by default, Secure Partitions are enabled according to test suites.
    if(TFM_PROFILE)
        include(${CMAKE_CURRENT_LIST_DIR}/profile_default_config.cmake)
    else()
        include(${CMAKE_CURRENT_LIST_DIR}/default_config.cmake)
    endif()
endif()

# Set up dependency Secure Partition configs
include(${CMAKE_CURRENT_LIST_DIR}/enable_dep_config.cmake)

# Set up variables for config_*.cmake.in
get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
    get_property(HELP_STRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
    get_property(CACHE_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)

    # Command-line options passing to Secure build
    if("${HELP_STRING}" MATCHES "variable specified on the command line")
        string(REGEX MATCH "^TEST_NS.*" _NS_TEST_FOUND "${CACHE_VAR}")
        if(NOT _NS_TEST_FOUND)
            # Skip NS test suite config options
            string(APPEND COMMAND_LINE_OPTIONS
                   "set(${CACHE_VAR}\r\n    ${${CACHE_VAR}}\r\n    CACHE ${CACHE_TYPE} \"${HELP_STRING}\")\r\n")
        endif()
    endif()

    # Secure test suites
    string(REGEX MATCH "^TEST_S.+" _S_TEST_FOUND "${CACHE_VAR}")
    if (_S_TEST_FOUND)
        format_string(FORMATTED_CACHE_VAR ${CACHE_VAR} 25 " ")
        format_string(FORMATTED_CACHE_VAL ${${CACHE_VAR}} 5 " ")
        string(APPEND S_TEST_CONFIG_OPTIONS
               "set(${FORMATTED_CACHE_VAR} ${FORMATTED_CACHE_VAL} CACHE ${CACHE_TYPE} \"${HELP_STRING}\")\r\n"
        )
    endif()

    # NS test suits
    string(REGEX MATCH "^TEST_NS.+" _NS_TEST_FOUND "${CACHE_VAR}")
    if (_NS_TEST_FOUND)
        format_string(FORMATTED_CACHE_VAR ${CACHE_VAR} 25 " ")
        format_string(FORMATTED_CACHE_VAL ${${CACHE_VAR}} 5 " ")
        string(APPEND NS_TEST_CONFIG_OPTIONS
               "set(${FORMATTED_CACHE_VAR} ${FORMATTED_CACHE_VAL} CACHE ${CACHE_TYPE} \"${HELP_STRING}\")\r\n"
        )
    endif()

    # Secure Partitions
    string(REGEX MATCH "^TFM_PARTITION_.+" _PARTITION_FOUND "${CACHE_VAR}")
    if (_PARTITION_FOUND)
        format_string(FORMATTED_CACHE_VAR ${CACHE_VAR} 40 " ")
        format_string(FORMATTED_CACHE_VAL ${${CACHE_VAR}} 5 " ")
        string(APPEND SP_CONFIG_OPTIONS
               "set(${FORMATTED_CACHE_VAR} ${FORMATTED_CACHE_VAL} CACHE ${CACHE_TYPE} \"${HELP_STRING}\")\r\n"
        )
    endif()
endforeach()

# This file is for Secure build
configure_file(${CMAKE_CURRENT_LIST_DIR}/config_spe.cmake.in
               ${CMAKE_BINARY_DIR}/config_spe.cmake
               @ONLY)

# This file is for NS build
configure_file(${CMAKE_CURRENT_LIST_DIR}/config_ns_test.cmake.in
               ${CMAKE_BINARY_DIR}/api_ns/config_ns_test.cmake
               @ONLY)

include(${CMAKE_CURRENT_LIST_DIR}/check_config.cmake)
