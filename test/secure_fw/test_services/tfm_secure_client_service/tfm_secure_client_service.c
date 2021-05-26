/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_secure_client_service.h"
#include "test_framework_integ_test.h"
#ifdef TFM_PSA_API
#include "psa/service.h"
#include "psa_manifest/tfm_secure_client_service.h"
#endif
/**
 * \brief Service initialisation function. No special initialisation is
 *        required.
 *
 * \return Returns 0 on success
 */
int32_t tfm_secure_client_service_init(void)
{
#ifdef TFM_PSA_API
    psa_signal_t signals;

    tfm_secure_client_service_sfn_run_tests();

    signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
    if (signals & TFM_SECURE_CLIENT_SRV_DUMMY_SIGNAL) {
        psa_panic();
    }
#endif
    return 0;
}

int32_t tfm_secure_client_service_sfn_run_tests(void)
{
    start_integ_test();
    return 0;
}
