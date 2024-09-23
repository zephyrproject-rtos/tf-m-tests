/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FPU_TESTS_LIB_H__
#define __FPU_TESTS_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

#define NR_FP_REG                   (32U)
#define NR_FP_CALLER_REG            (NR_FP_REG / 2)
#define NR_FP_CALLEE_REG            (NR_FP_REG / 2)
#define FP_BUF_SIZE                 (NR_FP_REG * sizeof(uint32_t))
#define FP_CALLER_BUF_SIZE          (NR_FP_CALLER_REG * sizeof(uint32_t))
#define FP_CALLEE_BUF_SIZE          (NR_FP_CALLEE_REG * sizeof(uint32_t))

/*
 * Introduction: Run functions in func_table sequentially with assembly
 * instructions.
 * Input:
 *  - func_table:  Table containing function addresses and input parameters.
 *  - func_return: Array containing function return values.
 *  - func_num:    Number of functions to be run.
 * Description: The format of func_table should be a function address followed
 * by its input parameter. If the function needs more than 1 input parameters,
 * input parameters need to be put into a struct. Then put the pointer of the
 * struct into func_table.
 * The length of func_return should be same as func_num. The return value of
 * each funtion will be stored in func_return after the function is finished.
 */
void fp_func_jump_template(uintptr_t *func_table, uintptr_t *func_return,
                           uint32_t func_num);

/* Dump FP caller registers to fp_caller_buffer. */
void dump_fp_caller(uint32_t *fp_caller_buffer);

/* Dump FP callee registers to fp_callee_buffer. */
void dump_fp_callee(uint32_t *fp_callee_buffer);

/* Change FP caller registers. */
void populate_caller_fp_regs(const uint32_t *expecting_caller_content);

/* Change FP callee registers. */
void populate_callee_fp_regs(const uint32_t *expecting_callee_content);

/* Trigger FPU test interrupt. */
uint32_t fpu_interrupt_trigger(uint32_t IRQ_NUM);

#ifdef __cplusplus
}
#endif

#endif /* __FPU_TESTS_LIB_H__ */
