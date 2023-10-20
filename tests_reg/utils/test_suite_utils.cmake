#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Temporarily unset NS test flags to skip NS test suites during S build.
# These flags will be recovered in post_secure_suite_build().
# Their values are kept in temporary local variables.
macro(pre_secure_suite_build)
    get_cmake_property(CACHE_VARS CACHE_VARIABLES)

    foreach(CACHE_VAR ${CACHE_VARS})
        string(REGEX MATCH "^TEST_NS.*" _NS_TEST_FOUND "${CACHE_VAR}")
        if(_NS_TEST_FOUND AND "${${CACHE_VAR}}")
            set(TEMP_${CACHE_VAR} ${${CACHE_VAR}})
            unset(${CACHE_VAR} CACHE)
        endif()
    endforeach()

    if(EXTRA_NS_TEST_SUITE_PATH)
        set(TEMP_EXTRA_NS_TEST_SUITE_PATH ${EXTRA_NS_TEST_SUITE_PATH})
        unset(EXTRA_NS_TEST_SUITE_PATH CACHE)
    endif()
endmacro()

# Recover test flags unset in pre_secure_suite_build().
# Their values are restored from temporary local variables and written back.
macro(post_secure_suite_build)
    get_cmake_property(TEMP_TEST_VARS VARIABLES)

    foreach(TEMP_TEST ${TEMP_TEST_VARS})
        string(REGEX MATCH "^TEMP_TEST_NS.*" _NS_TEST_FOUND "${TEMP_TEST}")
        if(_NS_TEST_FOUND AND "${${TEMP_TEST}}")
            string(REGEX REPLACE "^TEMP_" "" TEST_NAME ${TEMP_TEST})
            set(${TEST_NAME}    ON    CACHE BOOL "")
            unset(${TEMP_TEST})
        endif()
    endforeach()

    if(TEMP_EXTRA_NS_TEST_SUITE_PATH)
        set(EXTRA_NS_TEST_SUITE_PATH    ${TEMP_EXTRA_NS_TEST_SUITE_PATH}    CACHE PATH "")
        unset(TEMP_EXTRA_NS_TEST_SUITE_PATH)
    endif()
endmacro()

# Temporarily unset S test flags to skip S test suites during NS build.
# These flags will be recovered in post_ns_suite_build(). Their values are kept in temporary local
# variables.
macro(pre_ns_suite_build)
    get_cmake_property(CACHE_VARS CACHE_VARIABLES)

    foreach(CACHE_VAR ${CACHE_VARS})
        string(REGEX MATCH "^TEST_S.*" _S_TEST_FOUND "${CACHE_VAR}")
        if(_S_TEST_FOUND AND "${${CACHE_VAR}}")
            set(TEMP_${CACHE_VAR} ${${CACHE_VAR}})
            unset(${CACHE_VAR} CACHE)
        endif()
    endforeach()

    if(EXTRA_S_TEST_SUITE_PATH)
        set(TEMP_EXTRA_S_TEST_SUITE_PATH ${EXTRA_S_TEST_SUITE_PATH})
        unset(EXTRA_S_TEST_SUITE_PATH CACHE)
    endif()
endmacro()

# Recover test flags unset in pre_ns_suite_build().
# Their values are restored from temporary local variables and written back.
macro(post_ns_suite_build)
    get_cmake_property(TEMP_TEST_VARS VARIABLES)

    foreach(TEMP_TEST ${TEMP_TEST_VARS})
        string(REGEX MATCH "^TEMP_TEST_S.*" _S_TEST_FOUND "${TEMP_TEST}")
        if(_S_TEST_FOUND AND "${${TEMP_TEST}}")
            string(REGEX REPLACE "^TEMP_" "" TEST_NAME ${TEMP_TEST})
            set(${TEST_NAME}    ON    CACHE BOOL "")
            unset(${TEMP_TEST})
        endif()
    endforeach()

    if(TEMP_EXTRA_S_TEST_SUITE_PATH)
        set(EXTRA_S_TEST_SUITE_PATH    ${TEMP_EXTRA_S_TEST_SUITE_PATH}    CACHE PATH "")
        unset(TEMP_EXTRA_S_TEST_SUITE_PATH)
    endif()
endmacro()
