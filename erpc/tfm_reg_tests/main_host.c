/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "erpc_client_start.h"
#include "erpc_port.h"
#include "psa/client.h"
#include "tfm_erpc.h"

#include "non_secure_suites.h"

#if (!defined(ERPC_TRANSPORT_UART)) && (!defined(ERPC_TRANSPORT_TCP))
#include <stdlib.h>
#include <getopt.h>

#define ARGC_UART   3
#define ARGC_TCP    4
#endif /* !ERPC_TRANSPORT_UART && !ERPC_TRANSPORT_TCP */

int main(int argc, char *argv[])
{
    erpc_transport_t transport;

#if (!defined(ERPC_TRANSPORT_UART)) && (!defined(ERPC_TRANSPORT_TCP))
    int erpc_uart_flag = 0, erpc_tcp_flag = 0;
    char *uart_dev = NULL, *tcp_host = NULL, *tcp_port = NULL;
    struct option erpc_transport_options[] =
    {
        {"UART", no_argument, &erpc_uart_flag, 1},
        {"TCP", no_argument, &erpc_tcp_flag, 1},
        {0, 0, 0, 0}
    };
#endif /* !ERPC_TRANSPORT_UART && !ERPC_TRANSPORT_TCP */

#ifdef ERPC_TRANSPORT_UART
    transport = erpc_transport_serial_init(PORT_NAME, 115200);
#elif defined(ERPC_TRANSPORT_TCP)
    transport = erpc_transport_tcp_init(ERPC_HOST, ERPC_PORT, false);
#else
    /* Check if argc is correct */
    if (argc != ARGC_UART && argc != ARGC_TCP) {
        printf("Incorrect argument numbers.\r\n");
        printf("Please input --UART PORT or --TCP HOST PORT\r\n");
        return 1;
    }

    /* Loop check to set _flag and parse arguments */
    while (getopt_long(argc, argv, "", erpc_transport_options, NULL) != -1);
    if (!erpc_uart_flag && !erpc_tcp_flag) {
        printf("No valid transportation layer selected.\r\n");
        return 1;
    } else if (erpc_uart_flag && erpc_tcp_flag) {
        printf("UART and TCP cannot be set at the same time.\r\n");
        return 1;
    } else if (erpc_uart_flag) {
        if (argc - optind != 1) {
            printf("Incorrect argument numbers for --UART.\r\n");
            return 1;
        }
        uart_dev = argv[optind];
    } else if (erpc_tcp_flag) {
        if (argc - optind != 2) {
            printf("Incorrect argument numbers for --TCP.\r\n");
        return 1;
        }
        tcp_host = argv[optind];
        tcp_port = argv[optind + 1];
    }

    /* eRPC transport initialization */
    if (erpc_uart_flag) {
        printf("UART device is being set to %s\r\n", uart_dev);
        transport = erpc_transport_serial_init(uart_dev, 115200);
    } else if (erpc_tcp_flag) {
        printf("TCP connection is being set to %s:%s\r\n", tcp_host, tcp_port);
        transport = erpc_transport_tcp_init(tcp_host, atoi(tcp_port), false);
    }
#endif /* !ERPC_TRANSPORT_UART && !ERPC_TRANSPORT_TCP */

    if (!transport) {
        printf("eRPC transport initialization failed!\r\n");
        return 1;
    } else {
        printf("eRPC transport initialization succeeded.\r\n");
    }

    erpc_client_start(transport);

    printf("psa_framework_version: 0x%x\r\n", psa_framework_version());

    ns_reg_test_start();

    return 0;
}
