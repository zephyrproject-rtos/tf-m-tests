/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework.h"

#include "bl1_2_integration_tests.h"

#ifdef EXTRA_BL1_2_TEST_SUITE
#include "extra_bl1_2_tests.h"
#endif /* EXTRA_BL1_2_TEST_SUITE */

static struct test_suite_t test_suites[] = {

    {&register_testsuite_bl1_2_integration, 0, 0, 0},

#ifdef EXTRA_BL1_2_TEST_SUITE
    /* Bl1_2 extra test cases */
    {&register_testsuite_extra_bl1_2, 0, 0, 0},
#endif


    /* End of test suites */
    {0, 0, 0, 0}
};

enum test_suite_err_t run_bl1_2_testsuite(void)
{
    return run_test("BL1_2", test_suites);
}
