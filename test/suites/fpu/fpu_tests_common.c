/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fpu_tests_common.h"
#include "psa_manifest/sid.h"
#include "tfm_arch.h"

/**
 * Change FP registers.
 */
__attribute__((naked)) static void change_fp_in_client(void)
{
    __asm volatile(
        "mov       r0, #0x000000C0         \n"
        "vmov      s0, r0                  \n"
        "mov       r0, #0x000000C1         \n"
        "vmov      s1, r0                  \n"
        "mov       r0, #0x000000C2         \n"
        "vmov      s2, r0                  \n"
        "mov       r0, #0x000000C3         \n"
        "vmov      s3, r0                  \n"
        "mov       r0, #0x000000C4         \n"
        "vmov      s4, r0                  \n"
        "mov       r0, #0x000000C5         \n"
        "vmov      s5, r0                  \n"
        "mov       r0, #0x000000C6         \n"
        "vmov      s6, r0                  \n"
        "mov       r0, #0x000000C7         \n"
        "vmov      s7, r0                  \n"
        "mov       r0, #0x000000C8         \n"
        "vmov      s8, r0                  \n"
        "mov       r0, #0x000000C9         \n"
        "vmov      s9, r0                  \n"
        "mov       r0, #0x000000CA         \n"
        "vmov      s10, r0                 \n"
        "mov       r0, #0x000000CB         \n"
        "vmov      s11, r0                 \n"
        "mov       r0, #0x000000CC         \n"
        "vmov      s12, r0                 \n"
        "mov       r0, #0x000000CD         \n"
        "vmov      s13, r0                 \n"
        "mov       r0, #0x000000CE         \n"
        "vmov      s14, r0                 \n"
        "mov       r0, #0x000000CF         \n"
        "vmov      s15, r0                 \n"
        "mov       r0, #0x000000D0         \n"
        "vmov      s16, r0                 \n"
        "mov       r0, #0x000000D1         \n"
        "vmov      s17, r0                 \n"
        "mov       r0, #0x000000D2         \n"
        "vmov      s18, r0                 \n"
        "mov       r0, #0x000000D3         \n"
        "vmov      s19, r0                 \n"
        "mov       r0, #0x000000D4         \n"
        "vmov      s20, r0                 \n"
        "mov       r0, #0x000000D5         \n"
        "vmov      s21, r0                 \n"
        "mov       r0, #0x000000D6         \n"
        "vmov      s22, r0                 \n"
        "mov       r0, #0x000000D7         \n"
        "vmov      s23, r0                 \n"
        "mov       r0, #0x000000D8         \n"
        "vmov      s24, r0                 \n"
        "mov       r0, #0x000000D9         \n"
        "vmov      s25, r0                 \n"
        "mov       r0, #0x000000DA         \n"
        "vmov      s26, r0                 \n"
        "mov       r0, #0x000000DB         \n"
        "vmov      s27, r0                 \n"
        "mov       r0, #0x000000DC         \n"
        "vmov      s28, r0                 \n"
        "mov       r0, #0x000000DD         \n"
        "vmov      s29, r0                 \n"
        "mov       r0, #0x000000DE         \n"
        "vmov      s30, r0                 \n"
        "mov       r0, #0x000000DF         \n"
        "vmov      s31, r0                 \n"

        "bx        lr                      \n"
    );
}

/**
 * Check whether FP registers are restored correctly.
 * Return:
 *   1 - FP registers are restored correctly
 *   0 - FP registers are not restored correctly
 */
__attribute__((naked)) static bool check_fp_restored_client(void)
{
    __asm volatile(
        "mov       r3, #0                  \n"

        "vmov      r2, s0                  \n"
        "cmp       r2, 0x000000C0          \n"
        "bne       exit                    \n"

        "vmov      r2, s1                  \n"
        "cmp       r2, 0x000000C1          \n"
        "bne       exit                    \n"

        "vmov      r2, s2                  \n"
        "cmp       r2, 0x000000C2          \n"
        "bne       exit                    \n"

        "vmov      r2, s3                  \n"
        "cmp       r2, 0x000000C3          \n"
        "bne       exit                    \n"

        "vmov      r2, s4                  \n"
        "cmp       r2, 0x000000C4          \n"
        "bne       exit                    \n"

        "vmov      r2, s5                  \n"
        "cmp       r2, 0x000000C5          \n"
        "bne       exit                    \n"

        "vmov      r2, s6                  \n"
        "cmp       r2, 0x000000C6          \n"
        "bne       exit                    \n"

        "vmov      r2, s7                  \n"
        "cmp       r2, 0x000000C7          \n"
        "bne       exit                    \n"

        "vmov      r2, s8                  \n"
        "cmp       r2, 0x000000C8          \n"
        "bne       exit                    \n"

        "vmov      r2, s9                  \n"
        "cmp       r2, 0x000000C9          \n"
        "bne       exit                    \n"

        "vmov      r2, s10                 \n"
        "cmp       r2, 0x000000CA          \n"
        "bne       exit                    \n"

        "vmov      r2, s11                 \n"
        "cmp       r2, 0x000000CB          \n"
        "bne       exit                    \n"

        "vmov      r2, s12                 \n"
        "cmp       r2, 0x000000CC          \n"
        "bne       exit                    \n"

        "vmov      r2, s13                 \n"
        "cmp       r2, 0x000000CD          \n"
        "bne       exit                    \n"

        "vmov      r2, s14                 \n"
        "cmp       r2, 0x000000CE          \n"
        "bne       exit                    \n"

        "vmov      r2, s15                 \n"
        "cmp       r2, 0x000000CF          \n"
        "bne       exit                    \n"

        "vmov      r2, s16                 \n"
        "cmp       r2, 0x000000D0          \n"
        "bne       exit                    \n"

        "vmov      r2, s17                 \n"
        "cmp       r2, 0x000000D1          \n"
        "bne       exit                    \n"

        "vmov      r2, s18                 \n"
        "cmp       r2, 0x000000D2          \n"
        "bne       exit                    \n"

        "vmov      r2, s19                 \n"
        "cmp       r2, 0x000000D3          \n"
        "bne       exit                    \n"

        "vmov      r2, s20                 \n"
        "cmp       r2, 0x000000D4          \n"
        "bne       exit                    \n"

        "vmov      r2, s21                 \n"
        "cmp       r2, 0x000000D5          \n"
        "bne       exit                    \n"

        "vmov      r2, s22                 \n"
        "cmp       r2, 0x000000D6          \n"
        "bne       exit                    \n"

        "vmov      r2, s23                 \n"
        "cmp       r2, 0x000000D7          \n"
        "bne       exit                    \n"

        "vmov      r2, s24                 \n"
        "cmp       r2, 0x000000D8          \n"
        "bne       exit                    \n"

        "vmov      r2, s25                 \n"
        "cmp       r2, 0x000000D9          \n"
        "bne       exit                    \n"

        "vmov      r2, s26                 \n"
        "cmp       r2, 0x000000DA          \n"
        "bne       exit                    \n"

        "vmov      r2, s27                 \n"
        "cmp       r2, 0x000000DB          \n"
        "bne       exit                    \n"

        "vmov      r2, s28                 \n"
        "cmp       r2, 0x000000DC          \n"
        "bne       exit                    \n"

        "vmov      r2, s29                 \n"
        "cmp       r2, 0x000000DD          \n"
        "bne       exit                    \n"

        "vmov      r2, s30                 \n"
        "cmp       r2, 0x000000DE          \n"
        "bne       exit                    \n"

        "vmov      r2, s31                 \n"
        "cmp       r2, 0x000000DF          \n"
        "bne       exit                    \n"

        "mov       r3, #1                  \n"
        "exit:                             \n"
        "mov       r0, r3                  \n"

        "bx        lr                      \n"
    );
}

/**
 * Description: Test FP context protection after psa calls. Change FP registers
 * in FPU client/service partition separately, then check FP registers after
 * psa calls.
 * Expectation: FP registers in FPU client/service partition should be saved
 * and restored correctly.
 */
void tfm_fpu_test_fp_protection_psa_call(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;

    handle = psa_connect(TFM_FPU_SERVICE_CHECK_FP_REGISTER_SID,
                            TFM_FPU_SERVICE_CHECK_FP_REGISTER_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        ret->val = TEST_FAILED;
        return;
    }

    /* Change FP registers in secure thread */
    change_fp_in_client();

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status == PSA_SUCCESS) {
        /* FP registers should be restored */
        if (check_fp_restored_client()) {
            ret->val = TEST_PASSED;
        } else {
            ret->val = TEST_FAILED;
        }
    }

    psa_close(handle);
}

/**
 * Description: Clear FP registers in FPU service partition for next test.
 * Expectation: FP registers in FPU service partition should be cleared.
 */
void tfm_fpu_test_clear_service_fp_data(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;

    handle = psa_connect(TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SID,
                            TFM_FPU_SERVICE_CLEAR_FP_REGISTER_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        ret->val = TEST_FAILED;
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status == PSA_SUCCESS) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }

    psa_close(handle);
}

/**
 * Description: Test reliability of FP context protection after psa calls by
 * loops. Change FP registers in FPU client/service partition separately, then
 * check FP registers after psa calls.
 * Expectation: FP registers in FPU client/service partition should be saved
 * and restored correctly.
 */
void tfm_fpu_test_fp_protection_psa_call_loop(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    uint32_t itr;

    for (itr = 0; itr < LOOP_ITERATIONS; itr++) {
        TEST_LOG("  > Iteration %d of %d\r", itr + 1, LOOP_ITERATIONS);

        handle = psa_connect(TFM_FPU_SERVICE_CHECK_FP_REGISTER_SID,
                                TFM_FPU_SERVICE_CHECK_FP_REGISTER_VERSION);
        if (handle <= 0) {
            TEST_FAIL("The RoT Service has refused the connection!\r\n");
            ret->val = TEST_FAILED;
            return;
        }

        /* Change FP registers in secure thread */
        change_fp_in_client();

        status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
        if (status == PSA_SUCCESS) {
            /* FP registers should be restored */
            if (check_fp_restored_client()) {
                ret->val = TEST_PASSED;
            } else {
                ret->val = TEST_FAILED;
            }
        } else {
            ret->val = TEST_FAILED;
        }

        psa_close(handle);

        handle = psa_connect(TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SID,
                                TFM_FPU_SERVICE_CLEAR_FP_REGISTER_VERSION);
        if (handle <= 0) {
            TEST_LOG("The RoT Service has refused the connection!\r\n");
            ret->val = TEST_FAILED;
            return;
        }

        status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
        if (status == PSA_SUCCESS) {
            ret->val = TEST_PASSED;
        } else {
            ret->val = TEST_FAILED;
        }

        psa_close(handle);
    }

    ret->val = TEST_PASSED;
}
