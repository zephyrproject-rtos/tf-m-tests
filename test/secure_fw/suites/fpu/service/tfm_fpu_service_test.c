/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/client.h"
#include "psa/service.h"
#include "psa_manifest/tfm_fpu_service_test.h"
#include "tfm_api.h"
#include "tfm_hal_isolation.h"
#include "tfm_secure_api.h"
#include "tfm_sp_log.h"
#include "tfm_plat_test.h"
#include "device_definition.h"
#include "fpu_tests_common.h"

/*
 * Description: Service handler for checking FP register.
 * Expectation: FP callee registers should be restored correctly in service.
 */
static void fpu_service_check_fp_register(void)
{
    psa_msg_t msg;
    uint32_t fp_callee_buffer[NR_FP_CALLEE_REG] = {0};
    const uint32_t fp_clear_callee_content[NR_FP_CALLEE_REG] = {0};
    const uint32_t expecting_callee_content[NR_FP_CALLEE_REG] = {
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
        0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
    };

    psa_get(TFM_FPU_SERVICE_CHECK_FP_CALLEE_REGISTER_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        dump_fp_callee(fp_callee_buffer);
        if (!memcmp(fp_callee_buffer, fp_clear_callee_content,
                    FP_CALLEE_BUF_SIZE)) {
            populate_callee_fp_regs(expecting_callee_content);
            psa_reply(msg.handle, PSA_SUCCESS);
        } else {
            psa_reply(msg.handle, PSA_ERROR_GENERIC_ERROR);
        }
        break;
    case PSA_IPC_DISCONNECT:
        dump_fp_callee(fp_callee_buffer);
        if ((!memcmp(fp_callee_buffer, expecting_callee_content,
                     FP_CALLEE_BUF_SIZE))) {
            populate_callee_fp_regs(fp_clear_callee_content);
        }
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * Start S timer interrupt.
 * Expectation: S timer should be started.
 */
static void fpu_client_start_secure_timer(void)
{
    psa_msg_t msg;
    psa_status_t r;

    r = psa_get(TFM_FPU_SERVICE_START_S_TIMER_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        /* Start the timer */
        tfm_plat_test_secure_timer_start();
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * Read S timer reload value.
 * Expectation: S timer reload value should be read.
 */
static void fpu_client_check_secure_timer_triggered(void)
{
    psa_msg_t msg;
    psa_status_t r;
    int val;

    r = psa_get(TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        if (msg.out_size[0] != 0) {
            /* Read the timer reload value */
            if (tfm_plat_test_secure_timer_get_reload_value()
                            == REL_VALUE_FP_REGS_INVALIDATED) {
                val = S_TIMER_TRIGGERED;
            } else {
                val = S_TIMER_NOT_TRIGGERED;
            }
            psa_write(msg.handle, 0, &val, 1);
            r = PSA_SUCCESS;
        } else {
            r = PSA_ERROR_PROGRAMMER_ERROR;
        }
        psa_reply(msg.handle, r);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * Test FP context protection after NS interrupt.
 * Expectation: FP register in secure thread should be restored after NS
 * interrupt.
 */
int fpu_client_non_secure_interrupt_secure_test(void)
{
    psa_msg_t msg;
    psa_status_t r;
    static uint32_t i;

    uint32_t fp_caller_buffer[NR_FP_CALLER_REG] = {0};
    const uint32_t expecting_caller_content[NR_FP_CALLER_REG] = {
        0xB0000000, 0xB1000000, 0xB2000000, 0xB3000000,
        0xB4000000, 0xB5000000, 0xB6000000, 0xB7000000,
        0xB8000000, 0xB9000000, 0xBA000000, 0xBB000000,
        0xBC000000, 0xBD000000, 0xBE000000, 0xBF000000
    };
    uint32_t fp_callee_buffer[NR_FP_CALLEE_REG] = {0};
    const uint32_t expecting_callee_content[NR_FP_CALLEE_REG] = {
        0xC0000000, 0xC1000000, 0xC2000000, 0xC3000000,
        0xC4000000, 0xC5000000, 0xC6000000, 0xC7000000,
        0xC8000000, 0xC9000000, 0xCA000000, 0xCB000000,
        0xCC000000, 0xCD000000, 0xCE000000, 0xCF000000
    };

    r = psa_get(TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        /* Change FP regs */
        populate_caller_fp_regs(expecting_caller_content);
        populate_callee_fp_regs(expecting_callee_content);
        /* Start the timer */
        tfm_plat_test_non_secure_timer_start();
        LOG_DBGFMT("Wait for NS timer interrupt!\r\n");
        /* Spin here */
        while (1) {
            /* NS interrupt triggered */
            if (tfm_plat_test_non_secure_timer_get_reload_value()
                                == REL_VALUE_FP_REGS_INVALIDATED) {
                LOG_DBGFMT("S thread interrupted by NS timer interrupt!\r\n");
                break;
            } else {
                i++;
                if (i > LOOPS_NS_INT_TEST) {
                    LOG_DBGFMT("Time out: S thread is not interrupted!\r\n");
                    break;
                }
            }
        }
        if (i > LOOPS_NS_INT_TEST) {
            /* Error for time out */
            r = PSA_ERROR_GENERIC_ERROR;
        } else {
            /* FP register should be restored after NS interrupt. */
            dump_fp_caller(fp_caller_buffer);
            dump_fp_callee(fp_callee_buffer);
            if ((!memcmp(fp_caller_buffer, expecting_caller_content,
                         FP_CALLER_BUF_SIZE)) &&
                (!memcmp(fp_callee_buffer, expecting_callee_content,
                         FP_CALLEE_BUF_SIZE))) {
                r = PSA_SUCCESS;
            } else {
                r = PSA_ERROR_GENERIC_ERROR;
            }
        }
        /* Reply with status */
        psa_reply(msg.handle, r);
        break;
    default:
        psa_panic();
        break;
    }
}

/* FP service partition main thread. */
void fpu_service_test_main(void *param)
{
    uint32_t signals = 0;

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_FPU_SERVICE_CHECK_FP_CALLEE_REGISTER_SIGNAL) {
            fpu_service_check_fp_register();
        } else if (signals & TFM_FPU_SERVICE_START_S_TIMER_SIGNAL) {
            fpu_client_start_secure_timer();
        } else if (signals & TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_SIGNAL) {
            fpu_client_check_secure_timer_triggered();
        } else if (signals & TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_SIGNAL) {
            fpu_client_non_secure_interrupt_secure_test();
        } else {
            psa_panic();
        }
    }
}
