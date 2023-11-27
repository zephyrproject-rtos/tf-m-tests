/*
 * Copyright (c) 2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis_compiler.h"
#include "rtx_os.h"

/* This is an example OS configuration implementation for CMSIS-RTX */

/* OS Idle Thread implemented to put the CPU into low-power state until an event
 * occurs. The regular SysTick will still wake the CPU.
 */
__NO_RETURN void osRtxIdleThread(void *argument)
{
    (void)argument;

    for (;;) {
        __WFE();
    }
}
