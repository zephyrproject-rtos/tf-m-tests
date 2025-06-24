/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FPU_TESTS_COMMON_H__
#define __FPU_TESTS_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "test_framework.h"

#define LOOP_ITERATIONS             (300U)

#define PSA_CALL_PARAM_LEN                   (4U)

/*
 * Description: Clear FP registers to check basic FP register write/read
 * functionality. This is a shared test case for both S and NS sides.
 * Expectation: FP registers in FPU client partition should be cleared.
 */
void tfm_fpu_test_clear_client_fp_data(struct test_result_t *ret);

#ifdef __cplusplus
}
#endif

#endif /* __FPU_TESTS_COMMON_H__ */
