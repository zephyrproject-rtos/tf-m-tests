#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT CHECK_TFM_TESTS_VERSION)
    return()
endif()

find_package(Git)

execute_process(COMMAND "${GIT_EXECUTABLE}" status
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE RET_VALUE
                ERROR_QUIET OUTPUT_QUIET)
if(NOT RET_VALUE EQUAL 0)
    return()
endif()

if(NOT DEFINED RECOMMENDED_TFM_TESTS_VERSION)
    message(FATAL_ERROR
        " Recommended tf-m-tests version of TF-M is unknown.\n"
        " Please select tf-m-tests version specified in trusted-firmware-m/lib/ext/tf-m-tests/version.txt.")
endif()

# Fetch the full HEAD commit ID
execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE HEAD_FULL_COMMIT
                RESULT_VARIABLE RET_VALUE
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
# Fail to fetch the current tf-m-tests HEAD commit ID.
if(RET_VALUE)
    message(FATAL_ERROR
        " Failed to fetch the current tf-m-tests HEAD.\n"
        " Unable to check compatibility with TF-M.")
    return()
endif()

# Try to fetch the full commit ID of RECOMMENDED_TFM_TESTS_VERSION
# If the recommended tf-m-test version is a tag, it outputs the corresponding commit ID
# for comparison.
execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse ${RECOMMENDED_TFM_TESTS_VERSION}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE RECOMMENDED_FULL_COMMIT
                RESULT_VARIABLE RET_VALUE
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
# Fail to fetch the whole commit ID of RECOMMENDED_TFM_TESTS_VERSION.
# tf-m-tests is not updated yet and therefore behind of RECOMMENDED_TFM_TESTS_VERSION
if(RET_VALUE)
    message(FATAL_ERROR
        " Unknown recommended tf-m-tests version: ${RECOMMENDED_TFM_TESTS_VERSION}.\n"
        " Please update local tf-m-tests repo and checkout version *${RECOMMENDED_TFM_TESTS_VERSION}*.\n"
        " Otherwise, tf-m-tests and TF-M can be incompatible.")
endif()

# Check if tf-m-tests HEAD == commit ID recommended by TF-M
if("${HEAD_FULL_COMMIT}" STREQUAL "${RECOMMENDED_FULL_COMMIT}")
    message(VERBOSE "tf-m-tests HEAD matches the version recommended by TF-M")
    return()
endif()

# Check whether tf-m-tests commit ID recommended by TF-M is behind current tf-m-tests HEAD
execute_process(COMMAND "${GIT_EXECUTABLE}" merge-base --is-ancestor "${RECOMMENDED_TFM_TESTS_VERSION}" HEAD
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE VERSION_FROM_TFM_IS_OLDER
                ERROR_QUIET OUTPUT_QUIET)
# Users are using a newer version of tf-m-tests to verify TF-M
if(VERSION_FROM_TFM_IS_OLDER EQUAL 0)
    message(WARNING
        " Current tf-m-tests HEAD is ahead of the version *${RECOMMENDED_TFM_TESTS_VERSION}* recommended by TF-M.\n"
        " - If you are developing in tf-m-tests, please update tf-m-tests commit ID in\n"
        "   trusted-firmware-m/lib/ext/version.txt and upload that change to trusted-firmware-m.\n"
        " - If you are testing an older version of TF-M, please switch tf-m-tests to\n"
        "   version *${RECOMMENDED_TFM_TESTS_VERSION}*.\n"
        "   Build or tests might fail due to incompatible configurations.\n")
    return()
endif()

# Check whether tf-m-tests commit ID recommended by TF-M is ahead of current tf-m-tests HEAD
execute_process(COMMAND "${GIT_EXECUTABLE}" merge-base --is-ancestor HEAD "${RECOMMENDED_TFM_TESTS_VERSION}"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE VERSION_FROM_TFM_IS_NEWER
                ERROR_QUIET OUTPUT_QUIET)
# Users are using an out-of-date version of tf-m-tests to verify TF-M.
# Test coverage cannot be guaranteed. Block building in case that failures/defects are not detected.
if(VERSION_FROM_TFM_IS_NEWER EQUAL 0)
    message(FATAL_ERROR
        " Current tf-m-tests HEAD is behind the version *${RECOMMENDED_TFM_TESTS_VERSION}* recommended by TF-M.\n"
        " TF-M features might be not properly tested or covered by an older version of tf-m-tests.\n"
        " Please update tf-m-tests to version *${RECOMMENDED_TFM_TESTS_VERSION}* before verification.")
endif()

# The sequence of those 2 commits are unknown.
# Not sure what has happened. Throw a warning to notify users.
message(WARNING
    " Current tf-m-tests HEAD is different from the version *${RECOMMENDED_TFM_TESTS_VERSION}* recommended by TF-M.\n"
    " You might be working on a development branch diverged from the main branch.\n"
    " Build or tests might fail due to incompatible configurations.\n"
    " Suggest to rebase your commits on tf-m-tests main branch.\n")
