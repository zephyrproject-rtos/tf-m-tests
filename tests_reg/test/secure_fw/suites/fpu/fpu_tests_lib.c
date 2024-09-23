/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fpu_tests_lib.h"

__attribute__((naked))
void fp_func_jump_template(uintptr_t *func_table, uintptr_t *func_return,
                           uint32_t func_num)
{
    __asm volatile(
        "cmp       r2, #0           \n"
        "beq       jump_exit        \n"
        "push      {r4-r6, lr}      \n"
        "mov       r4, r0           \n"
        "mov       r5, r1           \n"
        "mov       r6, r2           \n"
    "jump_loop:                     \n"
        /* Load func address to r2.
         * Move r4 to the address of func param. */
        "ldr       r2, [r4], #4     \n"
        /* Load func param to r0.
         * Move r4 to the address of next func.*/
        "ldr       r0, [r4], #4     \n"
        /* Jump to target func. */
        "blx       r2               \n"
        /* Store return value of finished func.
         * Move r5 to store return value of next func. */
        "str       r0, [r5], #4     \n"
        "subs      r6, r6, #1       \n"
        "bne       jump_loop        \n"
        "pop       {r4-r6, pc}      \n"
    "jump_exit:                     \n"
        "bx        lr               \n"
    );
}

__attribute__((naked))
void dump_fp_caller(uint32_t *fp_caller_buffer)
{
    __asm volatile(
        "vstm      r0, {s0-s15}     \n"
        "bx        lr               \n"
    );
}

__attribute__((naked))
void dump_fp_callee(uint32_t *fp_callee_buffer)
{
    __asm volatile(
        "vstm      r0, {s16-s31}    \n"
        "bx        lr               \n"
    );
}

__attribute__((naked))
void populate_caller_fp_regs(const uint32_t *expecting_caller_content)
{
    __asm volatile(
        "vldm      r0, {s0-s15}     \n"
        "bx        lr               \n"
    );
}

__attribute__((naked))
void populate_callee_fp_regs(const uint32_t *expecting_callee_content)
{
    __asm volatile(
        "vldm      r0, {s16-s31}    \n"
        "bx        lr               \n"
    );
}

__attribute__((naked)) uint32_t fpu_interrupt_trigger(uint32_t IRQ_NUM){
    __asm volatile(
        "push    {r7, lr}                  \n"
        /* Software Trigger Interrupt Register address is 0xE000EF00. */
        "ldr     r7, =0xE000EF00           \n"
        "str     r0, [r7]                  \n"
        "dsb     #0xF                      \n"
        "pop     {r7, pc}                  \n"
    );
}
