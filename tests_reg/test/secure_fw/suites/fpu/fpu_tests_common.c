/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <string.h>
#include "fpu_tests_common.h"
#include "fpu_tests_lib.h"

void tfm_fpu_test_clear_client_fp_data(struct test_result_t *ret)
{
    uint32_t fp_caller_buffer[NR_FP_CALLER_REG] = {0};
    uint32_t fp_callee_buffer[NR_FP_CALLEE_REG] = {0};
    const uint32_t expecting_caller_content[NR_FP_CALLER_REG] = {0};
    const uint32_t expecting_callee_content[NR_FP_CALLEE_REG] = {0};

    uintptr_t func_table[] = {
        (uintptr_t)populate_caller_fp_regs, (uintptr_t)expecting_caller_content,
        (uintptr_t)populate_callee_fp_regs, (uintptr_t)expecting_callee_content,
        (uintptr_t)dump_fp_caller, (uintptr_t)fp_caller_buffer,
        (uintptr_t)dump_fp_callee, (uintptr_t)fp_callee_buffer
    };
    uintptr_t func_return[ARRAY_SIZE(func_table)/2] = {0};

    ret->val = TEST_PASSED;

    fp_func_jump_template(func_table, func_return, ARRAY_SIZE(func_table)/2);

    if (memcmp(fp_caller_buffer, expecting_caller_content,
               FP_CALLER_BUF_SIZE)) {
        TEST_FAIL("FP caller registers are not correctly cleared!");
    }

    if (memcmp(fp_callee_buffer, expecting_callee_content,
               FP_CALLEE_BUF_SIZE)) {
        TEST_FAIL("FP callee registers are not correctly cleared!");
    }
}
