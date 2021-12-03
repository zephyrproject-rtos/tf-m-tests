/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "fpu_ns_tests.h"
#include "../fpu_tests_common.h"

static struct test_t fpu_ns_tests[] = {
    {
        &tfm_fpu_test_clear_client_fp_data, "TFM_NS_FPU_TEST_1001",
        "Clear FP registers in FPU client partition",
        {TEST_PASSED}
    },
    {
        &tfm_fpu_test_fp_protection_psa_call, "TFM_NS_FPU_TEST_1002",
        "Test FP context protection after psa calls",
        {TEST_PASSED}
    }
};

void register_testsuite_ns_fpu_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(fpu_ns_tests) / sizeof(fpu_ns_tests[0]));

    set_testsuite("FPU non-secure interface test (TFM_NS_FPU_TEST_1XXX)",
                  fpu_ns_tests, list_size, p_test_suite);
}
