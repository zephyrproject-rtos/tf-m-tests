/*
 * Copyright (c) 2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __EXTRA_BL1_1_TESTS_H__
#define __EXTRA_BL1_1_TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "test_framework.h"

/**
 * \brief Register testsuite for the extra bl1_1 tests.
 *
 * \param[in] p_test_suite The test suite to be executed.
 */
void register_testsuite_extra_bl1_1(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __EXTRA_BL1_1_TESTS_H__ */
