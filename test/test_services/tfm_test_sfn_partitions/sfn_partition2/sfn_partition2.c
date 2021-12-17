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
#include "tfm_sfn_test_defs.h"

/**
 * \brief An example service implementation that writes the received data back
 *        to the client.
 */
psa_status_t tfm_sfn2_service1_sfn(const psa_msg_t* msg)
{
    psa_status_t status = PSA_ERROR_PROGRAMMER_ERROR;
    uint32_t i, str_size;
    uint8_t buf[SFN_SERVICE_BUFFER_LEN] = {'\0'};

    /* Decode the message */
    switch (msg->type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        status = PSA_SUCCESS;
        break;
    case PSA_IPC_CALL:
        LOG_DBGFMT("[SFN2 partition] Service in SFN2 called!\r\n");
        /* Read string message from the invec[0]. */
        str_size = msg->in_size[0];
        if (str_size != 0) {
            if (psa_read(msg->handle, 0, buf, SFN_SERVICE_BUFFER_LEN)
                                                        != str_size) {
                status = PSA_ERROR_PROGRAMMER_ERROR;
                break;
            }
        }
        /* Write the string message back to outvec[0]. */
        if (msg->out_size[0] >= str_size) {
            psa_write(msg->handle, 0, buf, str_size);
            status = PSA_SUCCESS;
        }
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
    LOG_DBGFMT("[SFN2 partition] SFN2 initialized.\r\n");
    return psa_call(TFM_SFN1_SERVICE1_HANDLE, PSA_IPC_CALL, NULL, 0, NULL, 0);
}
