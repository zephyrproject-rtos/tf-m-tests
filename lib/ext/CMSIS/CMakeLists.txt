#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
#
# This CMake script used in a split build only. In the legacy 'sinlge' build
# this file is ignoed. Please don't be confused.
#
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(CMSIS_5_RTX_V8MMN STATIC IMPORTED GLOBAL)
add_library(CMSIS_5_RTX_V8MMFN STATIC IMPORTED GLOBAL)
add_library(CMSIS_5_RTX_V8MBN STATIC IMPORTED GLOBAL)
add_library(CMSIS_5_RTX_CM3 STATIC IMPORTED GLOBAL)

set(CMSIS_LIBS_DIR ${CMAKE_CURRENT_LIST_DIR}/RTOS2/RTX/Library)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL GNU)
    set_target_properties(CMSIS_5_RTX_V8MMN  PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/GCC/libRTX_V8MMN.a)
    set_target_properties(CMSIS_5_RTX_V8MMFN PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/GCC/libRTX_V8MMFN.a)
    set_target_properties(CMSIS_5_RTX_V8MBN  PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/GCC/libRTX_V8MBN.a)
    set_target_properties(CMSIS_5_RTX_CM3    PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/GCC/libRTX_CM3.a)
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL ARMClang)
    set_target_properties(CMSIS_5_RTX_V8MMN  PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/ARM/RTX_V8MMN.lib)
    set_target_properties(CMSIS_5_RTX_V8MMFN PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/ARM/RTX_V8MMFN.lib)
    set_target_properties(CMSIS_5_RTX_V8MBN  PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/ARM/RTX_V8MBN.lib)
    set_target_properties(CMSIS_5_RTX_CM3    PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/ARM/RTX_CM3.lib)
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL IAR)
    add_library(CMSIS_5_RTX_V81MMN STATIC IMPORTED GLOBAL)
    set_target_properties(CMSIS_5_RTX_V8MMN  PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/IAR/RTX_V8MMN.a)
    set_target_properties(CMSIS_5_RTX_V81MMN PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/IAR/RTX_V81MMN.a)
    set_target_properties(CMSIS_5_RTX_V8MMFN PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/IAR/RTX_V8MMFN.a)
    set_target_properties(CMSIS_5_RTX_V8MBN  PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/IAR/RTX_V8MBN.a)
    set_target_properties(CMSIS_5_RTX_CM3    PROPERTIES IMPORTED_LOCATION ${CMSIS_LIBS_DIR}/IAR/RTX_CM3.a)
else()
    message(FATAL_ERROR "${CMAKE_C_COMPILER_ID} does not have CMSIS RTX static libraries set up")
endif()

if(NOT DEFINED CMSIS_5_RTX_LIB)
    # Select RTX library according to cpu/arch if platform does not select one.
    if(TFM_SYSTEM_ARCHITECTURE STREQUAL "armv6-m")
        set(CMSIS_5_RTX_LIB         "CMSIS_5_RTX_CM3")
    elseif(TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8-m.base")
        set(CMSIS_5_RTX_LIB         "CMSIS_5_RTX_V8MBN")
    elseif(TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8-m.main")
        if(CONFIG_TFM_ENABLE_FP OR CONFIG_TFM_ENABLE_MVE_FP)
            set(CMSIS_5_RTX_LIB     "CMSIS_5_RTX_V8MMFN")
        else()
            set(CMSIS_5_RTX_LIB     "CMSIS_5_RTX_V8MMN")
        endif()
    elseif(TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8.1-m.main")
        if("${CMAKE_C_COMPILER_ID}" STREQUAL "IAR")
            set(CMSIS_5_RTX_LIB     "CMSIS_5_RTX_V81MMN")
        elseif(CONFIG_TFM_ENABLE_FP OR CONFIG_TFM_ENABLE_MVE_FP)
            set(CMSIS_5_RTX_LIB     "CMSIS_5_RTX_V8MMFN")
        else()
            set(CMSIS_5_RTX_LIB     "CMSIS_5_RTX_V8MMN")
        endif()
    elseif(TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m4")
        set(CMSIS_5_RTX_LIB         "CMSIS_5_RTX_CM3")
    else()
        message(FATAL_ERROR
                "CMSIS_5_RTX_LIB library is NOT defined."
                "Unsupported processor ${TFM_SYSTEM_PROCESSOR} or architecture ${TFM_SYSTEM_ARCHITECTURE}."
                "Specify it in build command or update TFM_SYSTEM_PROCESSOR/TFM_SYSTEM_ARCHITECTURE.")
    endif()
endif()

add_library(RTX_OS INTERFACE)

target_sources(RTX_OS
    INTERFACE
        RTOS2/RTX/Config/RTX_Config.c
        RTOS2/RTX/Source/rtx_lib.c
)

target_include_directories(RTX_OS
    INTERFACE
        Core/Include
        RTOS2/Include
        RTOS2/RTX/Include
        RTOS2/RTX/Config
)

target_link_libraries(RTX_OS
    INTERFACE
        ${CMSIS_5_RTX_LIB}
        platform_ns         # for cmsis_compiler.h
)
