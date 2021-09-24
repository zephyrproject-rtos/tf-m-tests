
/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/service.h"
#include "psa_manifest/sfn_partition2.h"
#include "tfm_sp_log.h"
#include "psa_manifest/sid.h"

/**
 * \brief An example service implementation that prints out a message.
 */
psa_status_t tfm_sfn2_service1_sfn(const psa_msg_t* msg)
{
    psa_status_t status;

    if (msg == NULL) {
        psa_panic();
    }

    /* Decode the message */
    switch (msg->type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        status = PSA_SUCCESS;
        break;
    case PSA_IPC_CALL:
        LOG_INFFMT("[Example SFN2 partition] Service in SFN2 called!\r\n");
        status = PSA_SUCCESS;
        break;
    default:
        /* Invalid message type */
        status = PSA_ERROR_PROGRAMMER_ERROR;
        break;
    }
    return status;
}

/**
 * \brief SFN partition's initialization function is optional.
 */
psa_status_t sfn_partition_example2_init(void)
{
    LOG_INFFMT("[SFN2 partition] SFN2 initialized.\r\n");
    return psa_call(TFM_SFN1_SERVICE1_HANDLE, PSA_IPC_CALL,
                    NULL, 0, NULL, 0);
}
