/*
 * Copyright (c) 2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "erpc_port.h"
#include "erpc_client_start.h"
#include "tfm_erpc_psa_client_api.h"

int main(int argc, char *argv[])
{
    erpc_transport_t transport;

#ifdef ERPC_TRANSPORT_UART
    transport = erpc_transport_serial_init(PORT_NAME, 115200);
#elif defined(ERPC_TRANSPORT_TCP)
    transport = erpc_transport_tcp_init(ERPC_HOST, ERPC_PORT, false);
#else
#error "No valid transportation layer selected."
#endif

    if (!transport) {
        printf("eRPC transport init failed!\r\n");
        return 1;
    }

    erpc_client_start(transport);

    printf("psa_framework_version: %d\r\n", psa_framework_version());

    return 0;
}
