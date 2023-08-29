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

add_library(ns_multi_core STATIC
    PRIVATE
        tfm_multi_core_ns_api.c
        tfm_multi_core_psa_ns_api.c
        $<$<NOT:$<BOOL:${TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD}>>:tfm_ns_mailbox.c>
        $<$<AND:$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>,$<BOOL:${TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD}>>:tfm_ns_mailbox_thread.c>
        # NS RTOS specific implementation of NS mailbox
        $<$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>:tfm_ns_mailbox_rtos_api.c>
        $<$<BOOL:${TEST_NS_MULTI_CORE}>:tfm_ns_mailbox_test.c>
)

target_compile_definitions(ns_multi_core
    PUBLIC
        $<$<BOOL:${TFM_PARTITION_NS_AGENT_MAILBOX}>:TFM_PARTITION_NS_AGENT_MAILBOX>
        $<$<AND:$<BOOL:${TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD}>,$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>>:TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD>
)
