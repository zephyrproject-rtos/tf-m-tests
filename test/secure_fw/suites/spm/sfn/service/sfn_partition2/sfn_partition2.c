/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "client_api_test_defs.h"
#include "client_api_test_service.h"

#include "tfm_sp_log.h"
#include "psa/service.h"
#include "psa_manifest/sfn_partition2.h"
#include "psa_manifest/sid.h"

/**
 * \brief An example service implementation that writes the received data back
 *        to the client.
 */
psa_status_t tfm_sfn2_service1_sfn(const psa_msg_t* msg)
{
    psa_status_t status = PSA_ERROR_PROGRAMMER_ERROR;

    /* Decode the message */
    switch (msg->type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        status = PSA_SUCCESS;
        break;
    case CLIENT_API_TEST_TYPE_REQUEST_SRV:
        status = client_api_test_rot_srv(msg);
        break;
    default:
        /* Invalid message type */
        status = PSA_ERROR_NOT_SUPPORTED;
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
