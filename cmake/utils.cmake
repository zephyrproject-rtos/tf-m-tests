#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# A string calibrating routine to the specified length by
# appending to an input string a specified character.
#
# output - variable to return the calibrated string

function(format_string output input size filler)
    string(LENGTH ${input} length)
    foreach(i RANGE ${length} ${size})
        string(CONCAT input ${input} ${filler})
    endforeach()
    set(${output} ${input} PARENT_SCOPE)
endfunction()

# Prints formatted list of options with a title
#
# title - will be printed in a header
# options - list of CMake options to print (semicolon separated)
#
# Example:
# dump_options("Partitions" "TFM_PARTITION_CRYPTO; TFM_PARTITION_FIRMWARE_UPDATE ")
# will produce:
# -- -------- Partitions ---------------------
# -- TFM_PARTITION_CRYPTO                  ON
# -- TFM_PARTITION_FIRMWARE_UPDATE         OFF
# -- -----------------------------------------

function(dump_options title options)

    if (CONFIG_TFM_PARTITION_QUIET)
        return()
    endif()

    format_string(header "-------- ${title} " 50 "-")
    message(STATUS )
    message(STATUS "${header}")

    foreach (option ${options})
        string(STRIP ${option} option)
        # avoid errors on empty strings to tolerate ';' at the end of list
        if((DEFINED ${option}) AND NOT ${option} STREQUAL "")
            format_string(option_name ${option} 40 " ")
            message(STATUS "${option_name} ${${option}}")
        endif()
    endforeach()

    format_string(footer "-" 50 "-")
    message(STATUS "${footer}")
endfunction()

# Collect arguments via command line.
# cmd_line: the output argument to collect the arguments via command line
#
# Those command line arguments will be passed to ExternalProject_Add().
# Those arguments shall not be populated by the settings parsed inside each ExternalProject_Add()
function(collect_build_cmd_args cmd_line)

    get_cmake_property(CACHE_ARGS CACHE_VARIABLES)
    foreach(CACHE_ARG ${CACHE_ARGS})
        get_property(ARG_HELPSTRING CACHE "${CACHE_ARG}" PROPERTY HELPSTRING)
        if("${ARG_HELPSTRING}" MATCHES "variable specified on the command line")
            get_property(CACHE_ARG_TYPE CACHE ${CACHE_ARG} PROPERTY TYPE)
            set(ARG_VAL ${${CACHE_ARG}})

            # CMake automatically converts relative paths passed via command line into absolute
            # ones. Since external projects have different base directories compared to root
            # directory, relative paths will be incorrectly converted inside external projects.
            # Enforce all the relative paths into abosulte paths before collecting them in the
            # build command argument list.
            if(NOT ${ARG_VAL} STREQUAL "")
                if(IS_DIRECTORY ${ARG_VAL} AND NOT IS_ABSOLUTE ${ARG_VAL})
                    get_filename_component(ABS_PATH ${ARG_VAL} ABSOLUTE)
                    set(ARG_VAL ${ABS_PATH})
                endif()
            endif()
            list(APPEND TEMP_CMD_LINE "-D${CACHE_ARG}:${CACHE_ARG_TYPE}=${ARG_VAL}")
        endif()
    endforeach()

    set(${cmd_line} ${TEMP_CMD_LINE} PARENT_SCOPE)
endfunction()

function(tfm_invalid_config)
    if (${ARGV})
        string (REPLACE ";" " " ARGV_STRING "${ARGV}")
        string (REPLACE "STREQUAL"     "=" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "GREATER"      ">" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "LESS"         "<" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "VERSION_LESS" "<" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "EQUAL"        "=" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "IN_LIST"      "in" ARGV_STRING "${ARGV_STRING}")

        message(FATAL_ERROR "INVALID CONFIG: ${ARGV_STRING}")
    endif()
endfunction()
