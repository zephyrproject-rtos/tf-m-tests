/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TEST_LOG_H__
#define __TEST_LOG_H__

#if (DOMAIN_NS == 1) || defined(TEST_BL2)
#include "tfm_log_raw.h"
#else
#include "tfm_sp_log.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if (DOMAIN_NS == 1) || defined(TEST_BL2)
#define TEST_LOG(...) tfm_log_printf(__VA_ARGS__)
#else
#define TEST_LOG(...) tfm_sp_log_printf(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __TEST_LOG_H__ */
