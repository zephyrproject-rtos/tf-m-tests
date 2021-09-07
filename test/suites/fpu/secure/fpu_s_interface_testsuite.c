/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fpu_s_tests.h"
#include "../fpu_tests_common.h"

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

/**
 * Check invalidation of FP registers.
 */
__attribute__((naked)) static uint32_t check_fp_invalidated(void);

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
 * Description: Clear FP registers in FPU client partition for next test.
 * Expectation: FP registers in FPU client partition should be cleared.
 */
void tfm_fpu_test_clear_client_fp_data(struct test_result_t *ret)
{
    fpu_client_fp_clear_test();

    if (check_fp_invalidated()) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }
}

/**
 * Check invalidation of FP registers.
 * Return:
 *   1 - FP registers are invalidated
 *   0 - FP registers are not invalidated
 */
__attribute__((naked)) static uint32_t check_fp_invalidated(void)
{
    __asm volatile(
        "mov       r3, #1                  \n"
        "mov       r2, #0                  \n"

        "vadd.f32  s2, s1, s0              \n"
        "vadd.f32  s4, s3, s2              \n"
        "vadd.f32  s6, s5, s4              \n"
        "vadd.f32  s8, s7, s6              \n"
        "vadd.f32  s10, s9, s8            \n"
        "vadd.f32  s12, s11, s10           \n"
        "vadd.f32  s14, s13, s12           \n"
        "vadd.f32  s16, s15, s14           \n"
        "vadd.f32  s18, s17, s16           \n"
        "vadd.f32  s20, s19, s18           \n"
        "vadd.f32  s22, s21, s20           \n"
        "vadd.f32  s24, s23, s22           \n"
        "vadd.f32  s26, s25, s24           \n"
        "vadd.f32  s28, s27, s26           \n"
        "vadd.f32  s30, s29, s28           \n"
        "vadd.f32  s31, s29, s30           \n"
        "vcmp.f32  s31, #0.0               \n"
        "vmrs      APSR_nzcv, fpscr        \n"
        "beq       cleared                 \n"
        "mov       r3, r2                  \n"
        "cleared:                          \n"

        "mov       r0, r3                  \n"
        "bx        lr                      \n"
    );
}
