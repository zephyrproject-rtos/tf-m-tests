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
#include "tfm_sfn_test_defs.h"

/**
 * \brief An example stateless service implementation that prints
 *        a received magic number 0xFFFFABCD.
 */
psa_status_t tfm_sfn1_service1_sfn(const psa_msg_t* msg)
{
    psa_status_t status = PSA_ERROR_PROGRAMMER_ERROR;
    uint32_t in_data = 0;

    /* Decode the message */
    switch (msg->type) {
    case PSA_IPC_CALL:
        LOG_DBGFMT("[SFN1 partition] Service in SFN1 called!\r\n");
        /* This service can be called without input data or with a number. */
        if (msg->in_size[0] == sizeof(in_data)) {
            if (psa_read(msg->handle, 0, &in_data, sizeof(in_data))
                                                        != sizeof(in_data)) {
                status = PSA_ERROR_PROGRAMMER_ERROR;
                break;
            }
            if (in_data == SFN_SERVICE_MAGIC) {
                LOG_DBGFMT("[SFN1 partition] Correct input: %x\r\n", in_data);
            } else {
                LOG_DBGFMT("[SFN1 partition] Wrong input: %x\r\n", in_data);
                status = PSA_ERROR_PROGRAMMER_ERROR;
                break;
            }
        }
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
    LOG_DBGFMT("[SFN1 partition] SFN1 initialized.\r\n");
    return PSA_SUCCESS;
}
