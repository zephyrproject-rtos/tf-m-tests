/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_integ_test.h"
#include "test_framework_integ_test_helper.h"
#include "test_framework.h"

static struct test_suite_t test_suites[] = {

    /* End of test suites */
    {0, 0, 0, 0}
};

enum test_suite_err_t run_bl1_2_testsuite(void)
{
    return integ_test("BL1_2", test_suites);
}
