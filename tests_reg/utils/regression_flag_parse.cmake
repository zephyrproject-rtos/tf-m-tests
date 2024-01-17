#-------------------------------------------------------------------------------
# Copyright (c) 2021-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Set whether Secure and Non-secure regression tests are selected.
# If any regression test is selected, set corresponding TFM_S_REG_TEST/TEST_NS_REG_TEST to
# require TF-M build secure tests and test partitions.
#
# cmd_line: the output argument to collect the arguments via command line
#
function(parse_regression_flag cmd_line)

    set(TFM_NS_REG_TEST OFF)
    set(TFM_S_REG_TEST  OFF)

    get_cmake_property(CACHE_VARS CACHE_VARIABLES)

    # By default all non-secure regression tests are disabled.
    # If TEST_NS or TEST_NS_XXX flag is passed via command line and set to ON,
    # selected corresponding features to support non-secure regression tests.
    foreach(CACHE_VAR ${CACHE_VARS})
        string(REGEX MATCH "^TEST_NS.*" _NS_TEST_FOUND "${CACHE_VAR}")
        if (_NS_TEST_FOUND AND "${${CACHE_VAR}}")
            # TFM_NS_REG_TEST is a TF-M internal cmake flag to manage building
            # tf-m-tests non-secure regression tests related source
            set(TFM_NS_REG_TEST ON)
            break()
        endif()
    endforeach()

    foreach(CACHE_VAR ${CACHE_VARS})
        string(REGEX MATCH "^TEST_S.*" _S_TEST_FOUND "${CACHE_VAR}")
        if (_S_TEST_FOUND AND "${${CACHE_VAR}}")
            # TFM_S_REG_TEST is a TF-M internal cmake flag to manage building
            # tf-m-tests secure regression tests related source
            set(TFM_S_REG_TEST ON)
            break()
        endif()
    endforeach()

    # By default EXTRA_<NS/S>_TEST_SUITES_PATHS is not set, extra test is also an
    # out-of-tree build regression test, and if they are enabled,
    # TFM_<NS/S>_REG_TEST will be enabled.
    if (EXTRA_NS_TEST_SUITE_PATH)
        set(TFM_NS_REG_TEST ON)
    endif()

    if (EXTRA_S_TEST_SUITE_PATH)
        set(TFM_S_REG_TEST ON)
    endif()

    set(ns_reg_cmd      "-DTFM_NS_REG_TEST:BOOL=${TFM_NS_REG_TEST}")
    set(s_reg_cmd       "-DTFM_S_REG_TEST:BOOL=${TFM_S_REG_TEST}")
    set(${cmd_line}     "${${cmd_line}};${ns_reg_cmd};${s_reg_cmd}" PARENT_SCOPE)
    set(TFM_S_REG_TEST  ${TFM_S_REG_TEST} PARENT_SCOPE)
    set(TFM_NS_REG_TEST ${TFM_NS_REG_TEST} PARENT_SCOPE)

endfunction()
