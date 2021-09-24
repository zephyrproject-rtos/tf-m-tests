
/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/service.h"
#include "psa_manifest/sfn_partition1.h"
#include "tfm_sp_log.h"

/**
 * \brief An example service implementation that prints out a message.
 */
psa_status_t tfm_sfn1_service1_sfn(const psa_msg_t* msg)
{
    psa_status_t status;

    if (msg == NULL) {
        psa_panic();
    }

    /* Decode the message */
    switch (msg->type) {
    case PSA_IPC_CALL:
        LOG_INFFMT("[SFN1 partition] Service in SFN1 called!\r\n");
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
psa_status_t sfn_partition_example1_init(void)
{
    LOG_INFFMT("[SFN1 partition] SFN1 initialized.\r\n");
    return PSA_SUCCESS;
}
