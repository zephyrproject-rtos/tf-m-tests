/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "fpu_s_tests.h"
#include "../fpu_tests_common.h"
#include "tfm_memory_utils.h"

static struct test_t fpu_s_tests[] = {
    {
        &tfm_fpu_test_clear_client_fp_data, "TFM_S_FPU_TEST_1001",
        "Clear FP registers in FPU client partition",
        {TEST_PASSED}
    },
    {
        &tfm_fpu_test_fp_protection_psa_call, "TFM_S_FPU_TEST_1002",
        "Test FP context protection after psa calls",
        {TEST_PASSED}
    },
    {
        &tfm_fpu_test_clear_service_fp_data, "TFM_S_FPU_TEST_1003",
        "Clear FP registers in FPU service partition for next test",
        {TEST_PASSED}
    },
    {
        &tfm_fpu_test_fp_protection_psa_call_loop, "TFM_S_FPU_TEST_1004",
        "Test reliability of FP context protection after psa calls",
        {TEST_PASSED}
    }
};

void register_testsuite_s_fpu_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(fpu_s_tests) / sizeof(fpu_s_tests[0]));

    set_testsuite("FPU secure interface test (TFM_S_FPU_TEST_1XXX)",
                  fpu_s_tests, list_size, p_test_suite);
}

/**
 * Clear FP registers.
 */
__attribute__((naked)) static int fpu_client_fp_clear_test(void)
{
    __asm volatile(
        "eor       r0, r0, r0              \n"
        "vmov      s1, r0                  \n"
        "vmov      s2, r0                  \n"
        "vmov      s3, r0                  \n"
        "vmov      s4, r0                  \n"
        "vmov      s5, r0                  \n"
        "vmov      s6, r0                  \n"
        "vmov      s7, r0                  \n"
        "vmov      s8, r0                  \n"
        "vmov      s9, r0                  \n"
        "vmov      s10, r0                 \n"
        "vmov      s11, r0                 \n"
        "vmov      s12, r0                 \n"
        "vmov      s13, r0                 \n"
        "vmov      s14, r0                 \n"
        "vmov      s15, r0                 \n"
        "vmov      s16, r0                 \n"
        "vmov      s17, r0                 \n"
        "vmov      s18, r0                 \n"
        "vmov      s19, r0                 \n"
        "vmov      s20, r0                 \n"
        "vmov      s21, r0                 \n"
        "vmov      s22, r0                 \n"
        "vmov      s23, r0                 \n"
        "vmov      s24, r0                 \n"
        "vmov      s25, r0                 \n"
        "vmov      s26, r0                 \n"
        "vmov      s27, r0                 \n"
        "vmov      s28, r0                 \n"
        "vmov      s29, r0                 \n"
        "vmov      s30, r0                 \n"
        "vmov      s31, r0                 \n"

        "bx        lr                      \n"
    );
}

/**
 * Check invalidation of FP registers.
 * Return:
 *   1 - FP registers are invalidated
 *   0 - FP registers are not invalidated
 */
static bool check_fp_invalidated(void)
{
    uint32_t fp_buffer[NR_FP_REG] = {0};
    const uint32_t fp_expect[NR_FP_REG] = {0};

    __asm volatile(
        "vstm      %0, {S0-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

    if (!tfm_memcmp(fp_buffer, fp_expect, FP_BUF_SIZE)) {
        return true;
    } else {
        return false;
    }
}

/**
 * Description: Clear FP registers in FPU client partition for next test.
 * Expectation: FP registers in FPU client partition should be cleared.
 */
void tfm_fpu_test_clear_client_fp_data(struct test_result_t *ret)
{
    ret->val = TEST_FAILED;

    fpu_client_fp_clear_test();

    if (check_fp_invalidated()) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }
}
