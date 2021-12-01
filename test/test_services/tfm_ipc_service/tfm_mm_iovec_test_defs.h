/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_MM_IOVEC_TEST_DEFS_H__
#define __TFM_MM_IOVEC_TEST_DEFS_H__

#include <stdint.h>

/* MMIOVEC test service types */
#define INVEC_MAP_AND_UNMAP              (0)
#define OUTVEC_MAP_AND_UNMAP             (1)
#define OUTVEC_MAP_NOT_UNMAP             (2)

#define TFM_MMIOVEC_TEST_ERROR           (-257)

#define MMIOVEC_INPUT_DATA               (0xFFFFABCD)
#define MMIOVEC_OUTPUT_DATA              (0xA5)
#define MMIOVEC_TEST_VEC_LEN             (4)

#define MMIOVECT_TEST_INVEC    {MMIOVEC_INPUT_DATA, MMIOVEC_INPUT_DATA + 1, \
                                MMIOVEC_INPUT_DATA + 2, MMIOVEC_INPUT_DATA + 3}

#endif /* __TFM_MM_IOVEC_TEST_DEFS_H__ */
