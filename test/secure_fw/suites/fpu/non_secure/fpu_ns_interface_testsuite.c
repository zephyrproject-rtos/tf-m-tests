/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "fpu_ns_tests.h"
#include "../fpu_tests_common.h"
#include "os_wrapper/delay.h"
#include "tfm_psa_call_pack.h"

static void tfm_fpu_test_fp_protection_secure_interrupt(
                                                struct test_result_t *ret);
static void tfm_fpu_test_fp_protection_non_secure_interrupt(
                                                struct test_result_t *ret);
/* Test FP context protection after psa calls. */
static void tfm_fpu_test_fp_protection_ns_psa_call(struct test_result_t *ret);

/* Test reliability of FP context protection after psa calls by loops. */
static void tfm_fpu_test_fp_protection_ns_psa_call_loop(
                                                struct test_result_t *ret);

static struct test_t fpu_ns_tests[] = {
    {
        &tfm_fpu_test_clear_client_fp_data, "TFM_NS_FPU_TEST_1001",
        "Clear FP registers in FPU client partition"
    },
    {
        &tfm_fpu_test_fp_protection_ns_psa_call_loop, "TFM_NS_FPU_TEST_1002",
        "Test reliability of FP context protection after psa calls"
    },
    {
        &tfm_fpu_test_fp_protection_secure_interrupt, "TFM_NS_FPU_TEST_1003",
        "Test FP context protection in S interrupt after interrupt return"
    },
    {
        &tfm_fpu_test_fp_protection_non_secure_interrupt,
        "TFM_NS_FPU_TEST_1004",
        "Test FP context protection in S thread after NS interrupt"
    }
};

void register_testsuite_ns_fpu_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(fpu_ns_tests) / sizeof(fpu_ns_tests[0]));

    set_testsuite("FPU non-secure interface test (TFM_NS_FPU_TEST_1XXX)",
                  fpu_ns_tests, list_size, p_test_suite);
}

/**
 * \brief Test FP context protection in S interrupt. Change FP registers in
 * non-secure thread first, then change them in S interrupt. After interrupt
 * return, check FP context protection in non-secure thread.
 * Expectation: FP registers in S interrupt should not be view in non-secure
 * thread.
 */
static void tfm_fpu_test_fp_protection_secure_interrupt(
                                                struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    uint8_t outvec_data[1] = {0};
    struct psa_outvec outvecs[1] = {{outvec_data, sizeof(outvec_data[0])}};
    static uint8_t i;

    uint32_t fp_caller_buffer[NR_FP_CALLER_REG] = {0};
    uint32_t fp_callee_buffer[NR_FP_CALLEE_REG] = {0};
    const uint32_t expecting_caller_content[NR_FP_CALLER_REG] = {
        0xC0000000, 0xC1000000, 0xC2000000, 0xC3000000,
        0xC4000000, 0xC5000000, 0xC6000000, 0xC7000000,
        0xC8000000, 0xC9000000, 0xCA000000, 0xCB000000,
        0xCC000000, 0xCD000000, 0xCE000000, 0xCF000000
    };
    const uint32_t expecting_callee_content[NR_FP_CALLEE_REG] = {
        0xD0000000, 0xD1000000, 0xD2000000, 0xD3000000,
        0xD4000000, 0xD5000000, 0xD6000000, 0xD7000000,
        0xD8000000, 0xD9000000, 0xDA000000, 0xDB000000,
        0xDC000000, 0xDD000000, 0xDE000000, 0xDF000000
    };

    ret->val = TEST_FAILED;

    /* Change FP regs */
    populate_caller_fp_regs(expecting_caller_content);
    populate_callee_fp_regs(expecting_callee_content);

    /* Start the timer */
    handle = psa_connect(TFM_FPU_SERVICE_START_S_TIMER_SID,
                            TFM_FPU_SERVICE_START_S_TIMER_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return;
    }
    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status != PSA_SUCCESS) {
        return;
    }
    psa_close(handle);

    handle = psa_connect(TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_SID,
                            TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return;
    }
    /* Spin here */
    while (1) {
        /* Wait S interrupt triggered */
        os_wrapper_delay(WAIT_S_INT);

        status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
        if (status == PSA_SUCCESS) {
            /* S interrupt triggered */
            if (outvec_data[0] == S_TIMER_TRIGGERED) {
                break;
            } else {
                i++;
                if (i > LOOPS_S_INT_TEST) {
                    TEST_FAIL("Time out: NS thread not interrupted!\r\n");
                    psa_close(handle);
                    return;
                }
            }
        } else {
            TEST_FAIL("Check S interrupt triggered failed!\r\n");
            return;
        }
    }

    psa_close(handle);

    /* FP registers should be restored correctly after S interrupt */
    dump_fp_caller(fp_caller_buffer);
    dump_fp_callee(fp_callee_buffer);

    if ((!memcmp(fp_caller_buffer, expecting_caller_content,
                 FP_CALLER_BUF_SIZE)) &&
        (!memcmp(fp_callee_buffer, expecting_callee_content,
                 FP_CALLEE_BUF_SIZE))) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }
}

/**
 * \brief In secure thread, trigger non-secure timer interrupt, check FP
 *  context protection after NS interrupt.
 * Expectation: FP register in secure thread should be restored after NS
 *  interrupt.
 */
static void tfm_fpu_test_fp_protection_non_secure_interrupt(
                                                    struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    uint8_t *outvec_data[1] = {0};
    struct psa_outvec outvecs[1] = {{outvec_data, sizeof(outvec_data[0])}};

    ret->val = TEST_FAILED;

    handle = psa_connect(TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_SID,
                            TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
    if (status == PSA_SUCCESS) {
        ret->val = TEST_PASSED;
    }

    psa_close(handle);
}

/*
 * Description: Directly jump to tfm_psa_call_veneer from NS side.
 * Expectation: Return psa_status_t status code.
 */
__attribute__((naked)) static uint32_t fpu_ns_call(uintptr_t psa_call_param)
{
        __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                   \n"
#endif
        "   push    {r12, lr}              \n"
        "   mov     r12, r0                \n"
        /* Load params of tfm_psa_call_veneer into r0~r4. */
        "   ldr     r0, [r12], #4          \n"  /* psa handle */
        "   ldr     r1, [r12], #4          \n"  /* ctrl_param */
        "   ldr     r2, [r12], #4          \n"  /* in_vec */
        "   ldr     r3, [r12]              \n"  /* out_vec */
        "   blx     tfm_psa_call_veneer    \n"
        "   pop     {r12, pc}              \n"
    );
}

/*
 * Description: Test FP context protection after psa calls. Change FP registers
 * in FPU client/service partition separately, then check FP registers after
 * psa calls.
 * Expectation: FP registers in FPU client/service partition should be saved
 * and restored correctly.
 */
static void tfm_fpu_test_fp_protection_ns_psa_call(struct test_result_t *ret)
{
    psa_handle_t handle;
    uint32_t param;
    uint32_t psa_call_param[PSA_CALL_PARAM_LEN] = {0};
    uint32_t fp_callee_buffer[NR_FP_CALLEE_REG] = {0};
    const uint32_t expecting_callee_content[NR_FP_CALLEE_REG] = {
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF
    };

    uintptr_t func_table[] = {
        (uintptr_t)populate_callee_fp_regs, (uintptr_t)expecting_callee_content,
        (uintptr_t)fpu_ns_call, (uintptr_t)psa_call_param,
        (uintptr_t)dump_fp_callee, (uintptr_t)fp_callee_buffer
    };
    uintptr_t func_return[ARRAY_SIZE(func_table)/2] = {0};

    ret->val = TEST_PASSED;

    handle = psa_connect(TFM_FPU_SERVICE_CHECK_FP_CALLEE_REGISTER_SID,
                         TFM_FPU_SERVICE_CHECK_FP_CALLEE_REGISTER_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("PSA Connect fail!");
        return;
    }

    param = PARAM_PACK(PSA_IPC_CALL, 0, 0);
    psa_call_param[0] = (uint32_t)handle;
    psa_call_param[1] = param;

    fp_func_jump_template(func_table, func_return, ARRAY_SIZE(func_table)/2);

    if ((psa_status_t)func_return[1] != PSA_SUCCESS) {
        TEST_FAIL("FP callee registers check fail in service!");
    }

    if (memcmp(fp_callee_buffer, expecting_callee_content,
               FP_CALLEE_BUF_SIZE)) {
        TEST_FAIL("FP callee registers are not correctly restored in client!");
    }

    psa_close(handle);
}

/*
 * Description: Test reliability of FP context protection after psa calls by
 * loops. Change FP callee registers in FPU client/service partition separately,
 * then check FP callee registers after psa calls.
 * Expectation: FP callee registers in FPU client/service partition should be
 * saved and restored correctly.
 */
static void tfm_fpu_test_fp_protection_ns_psa_call_loop(
                                                struct test_result_t *ret)
{
    uint32_t itr;

    for (itr = 0; itr < LOOP_ITERATIONS; itr++) {
        TEST_LOG("  > Iteration %d of %d\r", itr + 1, LOOP_ITERATIONS);

        tfm_fpu_test_fp_protection_ns_psa_call(ret);
    }
}
