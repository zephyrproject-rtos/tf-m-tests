/*
 * Copyright (c) 2019-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_LOG_H__
#define __TFM_LOG_H__

#include "tfm_log_raw.h"

/* Functions and macros in this file is for 'thread mode' usage. */

#ifdef TFM_NS_LOG
#define LOG_MSG(...) tfm_log_printf(__VA_ARGS__)
#else
#define LOG_MSG(...)
#endif

#endif /* __TFM_LOG_H__ */
