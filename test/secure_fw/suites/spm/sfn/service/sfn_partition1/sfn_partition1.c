/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/service.h"
#include "psa_manifest/sfn_partition1.h"
#include "spm_test_defs.h"
#include "tfm_sp_log.h"
#include "client_api_test_defs.h"
#include "client_api_test_service.h"
#if PSA_FRAMEWORK_HAS_MM_IOVEC
#include "tfm_mmiovec_test_service.h"
#endif

/**
 * \brief An example stateless service implementation that prints
 *        a received magic number 0xFFFFABCD.
 */
psa_status_t tfm_sfn1_service1_sfn(const psa_msg_t* msg)
{
    psa_status_t status = PSA_ERROR_PROGRAMMER_ERROR;

    /* Decode the message */
    switch (msg->type) {
    case PSA_IPC_CALL:
        LOG_DBGFMT("[SFN1 partition] Service in SFN1 called!\r\n");
        status = PSA_SUCCESS;
        break;
    case CLIENT_API_TEST_TYPE_REQUEST_SRV:
        status = client_api_test_rot_srv(msg);
        break;
#if PSA_FRAMEWORK_HAS_MM_IOVEC
    case INVEC_MAP_AND_UNMAP:
        status = test_service_mmiovec_invec(msg);
        break;
    case OUTVEC_MAP_AND_UNMAP:
        status = test_service_mmiovec_outvec(msg);
        break;
    case OUTVEC_MAP_NOT_UNMAP:
        status = test_service_outvec_not_unmap(msg);
        break;
#endif
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
psa_status_t sfn_partition_example1_init(void)
{
    LOG_DBGFMT("[SFN1 partition] SFN1 initialized.\r\n");
    return PSA_SUCCESS;
}
