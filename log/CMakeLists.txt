#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

############################# Ns Log ###########################################
add_library(tfm_log STATIC)

target_sources(tfm_log
    PRIVATE
        tfm_log_raw.c
)

target_include_directories(tfm_log
    PUBLIC
        .
)

target_link_libraries(tfm_log
    PRIVATE
        platform_common_interface
)
