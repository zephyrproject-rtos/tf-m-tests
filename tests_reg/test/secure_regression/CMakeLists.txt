#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Check configuration.
# Config check shall be performed after all configurations complete.
# Difficult to insert a hook to perform test config check inside configuration sequence.
# Put the check here before secure regression tests start.
include(${CMAKE_CURRENT_LIST_DIR}/../config/check_config.cmake)

if(TFM_S_REG_TEST OR TEST_BL2 OR TEST_BL1_1 OR TEST_BL1_2)
    # In secure build, if a service/test looks for top-level directories, it will be a very long path.
    # Define variables of top-level directories to shorten the long paths.
    # Regresstion test directory
    set(REG_TEST_DIR          ${CMAKE_CURRENT_LIST_DIR}/../)
    # Root directory of tf-m-tests
    set(TFM_TESTS_ROOT_DIR    ${CMAKE_CURRENT_LIST_DIR}/../../../)

    list(APPEND CMAKE_MODULE_PATH ${TFM_TESTS_ROOT_DIR}/cmake)
    list(APPEND CMAKE_MODULE_PATH ${REG_TEST_DIR}/../utils)
    include(utils)

    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../framework ${CMAKE_CURRENT_BINARY_DIR}/framework)
endif()

if(TFM_S_REG_TEST)
    include(secure_fw.cmake)
endif()

if(TEST_BL2)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../bl2 ${CMAKE_CURRENT_BINARY_DIR}/bl2)
endif()

if(TEST_BL1_1 OR TEST_BL1_2)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../bl1 ${CMAKE_CURRENT_BINARY_DIR}/bl1)
endif()

# NS test suites
get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
    get_property(HELP_STRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
    get_property(CACHE_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)

    string(REGEX MATCH "^TEST_NS.+" _NS_TEST_FOUND "${CACHE_VAR}")
    if (_NS_TEST_FOUND)
        format_string(FORMATTED_CACHE_VAR ${CACHE_VAR} 25 " ")
        format_string(FORMATTED_CACHE_VAL ${${CACHE_VAR}} 5 " ")
        string(APPEND NS_TEST_CONFIG_OPTIONS
               "set(${FORMATTED_CACHE_VAR} ${FORMATTED_CACHE_VAL} CACHE ${CACHE_TYPE} \"${HELP_STRING}\")\r\n"
        )
    endif()
endforeach()

# Specify TFM_NS_REG_TEST
string(APPEND NS_TEST_CONFIG_OPTIONS
       "set(TFM_NS_REG_TEST \"${TFM_NS_REG_TEST}\")\r\n"
)

# Extra NS test sute path
string(APPEND NS_TEST_CONFIG_OPTIONS
       "set(EXTRA_NS_TEST_SUITE_PATH \"${EXTRA_NS_TEST_SUITE_PATH}\"  CACHE PATH \"List of extra non-secure test suites directories. An extra test suite folder contains source code, CMakeLists.txt and cmake configuration file\")\r\n"
)

# This file is for NS build
configure_file(${CMAKE_CURRENT_LIST_DIR}/../config/config_ns_test.cmake.in
               ${CMAKE_INSTALL_PREFIX}/config/config_ns_test.cmake
               @ONLY)
