/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "psa/client.h"
#include "psa/service.h"
#include "psa_manifest/tfm_fpu_service_test.h"
#include "tfm_api.h"
#include "tfm_hal_isolation.h"
#include "tfm_secure_api.h"

/* Define the return status */
#define FPU_SP_TEST_SUCCESS     (0)
#define FPU_SP_TEST_FAILED      (-1)

/*
 * Fixme: Temporarily implement abort as infinite loop,
 * will replace it later.
 */
static void tfm_abort(void)
{
    while (1)
        ;
}

/**
 * Clear FP registers.
 */
__attribute__((naked)) static void clear_fp_regs(void)
{
    __asm volatile(
        "eor       r0, r0, r0              \n"
        "vmov      s0, r0                  \n"
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
 * Check whether FP registers are restored correctly.
 * Return:
 *   1 - FP registers are restored correctly
 *   0 - FP registers are not restored correctly
 */
__attribute__((naked)) static bool check_fp_restored_service(void)
{
    __asm volatile(
        "mov       r3, #0                  \n"

        "vmov      r2, s0                  \n"
        "cmp       r2, 0x000000E0          \n"
        "bne       quit                    \n"

        "vmov      r2, s1                  \n"
        "cmp       r2, 0x000000E1          \n"
        "bne       quit                    \n"

        "vmov      r2, s2                  \n"
        "cmp       r2, 0x000000E2          \n"
        "bne       quit                    \n"

        "vmov      r2, s3                  \n"
        "cmp       r2, 0x000000E3          \n"
        "bne       quit                    \n"

        "vmov      r2, s4                  \n"
        "cmp       r2, 0x000000E4          \n"
        "bne       quit                    \n"

        "vmov      r2, s5                  \n"
        "cmp       r2, 0x000000E5          \n"
        "bne       quit                    \n"

        "vmov      r2, s6                  \n"
        "cmp       r2, 0x000000E6          \n"
        "bne       quit                    \n"

        "vmov      r2, s7                  \n"
        "cmp       r2, 0x000000E7          \n"
        "bne       quit                    \n"

        "vmov      r2, s8                  \n"
        "cmp       r2, 0x000000E8          \n"
        "bne       quit                    \n"

        "vmov      r2, s9                  \n"
        "cmp       r2, 0x000000E9          \n"
        "bne       quit                    \n"

        "vmov      r2, s10                 \n"
        "cmp       r2, 0x000000EA          \n"
        "bne       quit                    \n"

        "vmov      r2, s11                 \n"
        "cmp       r2, 0x000000EB          \n"
        "bne       quit                    \n"

        "vmov      r2, s12                 \n"
        "cmp       r2, 0x000000EC          \n"
        "bne       quit                    \n"

        "vmov      r2, s13                 \n"
        "cmp       r2, 0x000000ED          \n"
        "bne       quit                    \n"

        "vmov      r2, s14                 \n"
        "cmp       r2, 0x000000EE          \n"
        "bne       quit                    \n"

        "vmov      r2, s15                 \n"
        "cmp       r2, 0x000000EF          \n"
        "bne       quit                    \n"

        "vmov      r2, s16                 \n"
        "cmp       r2, 0x000000F0          \n"
        "bne       quit                    \n"

        "vmov      r2, s17                 \n"
        "cmp       r2, 0x000000F1          \n"
        "bne       quit                    \n"

        "vmov      r2, s18                 \n"
        "cmp       r2, 0x000000F2          \n"
        "bne       quit                    \n"

        "vmov      r2, s19                 \n"
        "cmp       r2, 0x000000F3          \n"
        "bne       quit                    \n"

        "vmov      r2, s20                 \n"
        "cmp       r2, 0x000000F4          \n"
        "bne       quit                    \n"

        "vmov      r2, s21                 \n"
        "cmp       r2, 0x000000F5          \n"
        "bne       quit                    \n"

        "vmov      r2, s22                 \n"
        "cmp       r2, 0x000000F6          \n"
        "bne       quit                    \n"

        "vmov      r2, s23                 \n"
        "cmp       r2, 0x000000F7          \n"
        "bne       quit                    \n"

        "vmov      r2, s24                 \n"
        "cmp       r2, 0x000000F8          \n"
        "bne       quit                    \n"

        "vmov      r2, s25                 \n"
        "cmp       r2, 0x000000F9          \n"
        "bne       quit                    \n"

        "vmov      r2, s26                 \n"
        "cmp       r2, 0x000000FA          \n"
        "bne       quit                    \n"

        "vmov      r2, s27                 \n"
        "cmp       r2, 0x000000FB          \n"
        "bne       quit                    \n"

        "vmov      r2, s28                 \n"
        "cmp       r2, 0x000000FC          \n"
        "bne       quit                    \n"

        "vmov      r2, s29                 \n"
        "cmp       r2, 0x000000FD          \n"
        "bne       quit                    \n"

        "vmov      r2, s30                 \n"
        "cmp       r2, 0x000000FE          \n"
        "bne       quit                    \n"

        "vmov      r2, s31                 \n"
        "cmp       r2, 0x000000FF          \n"
        "bne       quit                    \n"

        "mov       r3, #1                  \n"
        "quit:                             \n"
        "mov       r0, r3                  \n"

        "bx        lr                      \n"
    );
}

/**
 * Change FP registers in FP service partition.
 */
__attribute__((naked)) static void change_fp_in_service(void)
{
    __asm volatile(
        "mov       r0, #0x000000E0         \n"
        "vmov      s0, r0                  \n"
        "mov       r0, #0x000000E1         \n"
        "vmov      s1, r0                  \n"
        "mov       r0, #0x000000E2         \n"
        "vmov      s2, r0                  \n"
        "mov       r0, #0x000000E3         \n"
        "vmov      s3, r0                  \n"
        "mov       r0, #0x000000E4         \n"
        "vmov      s4, r0                  \n"
        "mov       r0, #0x000000E5         \n"
        "vmov      s5, r0                  \n"
        "mov       r0, #0x000000E6         \n"
        "vmov      s6, r0                  \n"
        "mov       r0, #0x000000E7         \n"
        "vmov      s7, r0                  \n"
        "mov       r0, #0x000000E8         \n"
        "vmov      s8, r0                  \n"
        "mov       r0, #0x000000E9         \n"
        "vmov      s9, r0                  \n"
        "mov       r0, #0x000000EA         \n"
        "vmov      s10, r0                 \n"
        "mov       r0, #0x000000EB         \n"
        "vmov      s11, r0                 \n"
        "mov       r0, #0x000000EC         \n"
        "vmov      s12, r0                 \n"
        "mov       r0, #0x000000ED         \n"
        "vmov      s13, r0                 \n"
        "mov       r0, #0x000000EE         \n"
        "vmov      s14, r0                 \n"
        "mov       r0, #0x000000EF         \n"
        "vmov      s15, r0                 \n"
        "mov       r0, #0x000000F0         \n"
        "vmov      s16, r0                 \n"
        "mov       r0, #0x000000F1         \n"
        "vmov      s17, r0                 \n"
        "mov       r0, #0x000000F2         \n"
        "vmov      s18, r0                 \n"
        "mov       r0, #0x000000F3         \n"
        "vmov      s19, r0                 \n"
        "mov       r0, #0x000000F4         \n"
        "vmov      s20, r0                 \n"
        "mov       r0, #0x000000F5         \n"
        "vmov      s21, r0                 \n"
        "mov       r0, #0x000000F6         \n"
        "vmov      s22, r0                 \n"
        "mov       r0, #0x000000F7         \n"
        "vmov      s23, r0                 \n"
        "mov       r0, #0x000000F8         \n"
        "vmov      s24, r0                 \n"
        "mov       r0, #0x000000F9         \n"
        "vmov      s25, r0                 \n"
        "mov       r0, #0x000000FA         \n"
        "vmov      s26, r0                 \n"
        "mov       r0, #0x000000FB         \n"
        "vmov      s27, r0                 \n"
        "mov       r0, #0x000000FC         \n"
        "vmov      s28, r0                 \n"
        "mov       r0, #0x000000FD         \n"
        "vmov      s29, r0                 \n"
        "mov       r0, #0x000000FE         \n"
        "vmov      s30, r0                 \n"
        "mov       r0, #0x000000FF         \n"
        "vmov      s31, r0                 \n"

        "bx        lr                      \n"
    );
}

/**
 * Check whether FP registers is invalidated.
 */
__attribute__((naked)) static bool is_fp_cleaned(void)
{
    __asm volatile(
        "mov       r3, #1                  \n"
        "mov       r2, #0                  \n"
        "vadd.f32  s2, s1, s0              \n"
        "vadd.f32  s4, s3, s2              \n"
        "vadd.f32  s6, s5, s4              \n"
        "vadd.f32  s8, s7, s6              \n"
        "vadd.f32  s10, s9, s8             \n"
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
        "beq       cleaned                 \n"
        "mov       r3, r2                  \n"
        "cleaned:                          \n"
        "mov       r0, r3                  \n"

        "bx       lr                      \n"
    );
}

/**
 * Service handler for clear FP register.
 */
static void fpu_service_clear_fp_register(void)
{
    psa_msg_t msg;
    psa_status_t r;

    psa_get(TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        if (check_fp_restored_service()) {
            clear_fp_regs();
            r = PSA_SUCCESS;
        } else {
            r = PSA_ERROR_INVALID_ARGUMENT;
        }
        psa_reply(msg.handle, r);
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        /* Should not come here */
        tfm_abort();
        break;
    }
}

/**
 * Service handler for checking FP register.
 */
static void fpu_service_check_fp_register(void)
{
    psa_msg_t msg;

    psa_get(TFM_FPU_SERVICE_CHECK_FP_REGISTER_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        if (is_fp_cleaned()) {
            change_fp_in_service();
            psa_reply(msg.handle, PSA_SUCCESS);
        } else {
            psa_reply(msg.handle, PSA_ERROR_GENERIC_ERROR);
        }
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        /* Should not come here */
        tfm_abort();
        break;
    }
}

/**
 * FP service partition main thread.
 */
void fpu_service_test_main(void *param)
{
    uint32_t signals = 0;

    clear_fp_regs();
    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SIGNAL) {
            fpu_service_clear_fp_register();
        } else if (signals & TFM_FPU_SERVICE_CHECK_FP_REGISTER_SIGNAL) {
            fpu_service_check_fp_register();
        } else {
            /* Should not come here */
            tfm_abort();
        }
    }
}
