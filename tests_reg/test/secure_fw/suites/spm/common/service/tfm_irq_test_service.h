/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_IRQ_TEST_SERVICE_H__
#define __TFM_IRQ_TEST_SERVICE_H__

#include <stdint.h>
#include "psa/service.h"

#ifdef TFM_PARTITION_SLIH_TEST
void slih_test_case_1(const psa_msg_t *msg, psa_signal_t timer_irq_signal);
#endif /* TFM_PARTITION_SLIH_TEST */

#ifdef TFM_PARTITION_FLIH_TEST
psa_flih_result_t tfm_flih_test_timer_handler(void);
void flih_test_case_1(const psa_msg_t *msg, psa_signal_t timer_irq_signal);
void flih_test_case_2(const psa_msg_t *msg, psa_signal_t timer_irq_signal);
#endif /* TFM_PARTITION_FLIH_TEST */

#endif /* __TFM_IRQ_TEST_SERVICE_H__ */
