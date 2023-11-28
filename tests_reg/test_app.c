/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_app.h"
#include "tfm_log.h"
#ifdef TFM_NS_REG_TEST
#include "non_secure_suites.h"
#endif

/**
 * \brief Services test thread
 *
 */
__attribute__((noreturn))
void test_app(void *argument)
{
    UNUSED_VARIABLE(argument);

#ifdef TFM_NS_REG_TEST
    ns_reg_test_start();
#endif

    /* Output EOT char for test environments like FVP. */
    LOG_MSG("\x04");

    /* End of test */
    for (;;) {
    }
}
