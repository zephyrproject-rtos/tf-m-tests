/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "ipc_s_tests.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"
#include "test_framework.h"
#include "client_api_tests.h"

/* List of tests */
static void tfm_ipc_test_1001(struct test_result_t *ret);
static void tfm_ipc_test_1002(struct test_result_t *ret);
static void tfm_ipc_test_1004(struct test_result_t *ret);
static void tfm_ipc_test_1012(struct test_result_t *ret);

static struct test_t ipc_veneers_tests[] = {
    {&tfm_ipc_test_1001, "TFM_S_IPC_TEST_1001",
     "Get PSA framework version"},
    {&tfm_ipc_test_1002, "TFM_S_IPC_TEST_1002",
     "Get version of an RoT Service"},
    {&tfm_ipc_test_1004, "TFM_S_IPC_TEST_1004",
     "Request connection-based RoT Service"},
    {&tfm_ipc_test_1012, "TFM_S_IPC_TEST_1012",
     "Request stateless service"},
};

void register_testsuite_s_ipc_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(ipc_veneers_tests) / sizeof(ipc_veneers_tests[0]));

    set_testsuite("IPC secure interface test (TFM_S_IPC_TEST_1XXX)",
                  ipc_veneers_tests, list_size, p_test_suite);
}

/**
 * \brief Retrieve the version of the PSA Framework API.
 *
 * \note This is a functional test only and doesn't
 *       mean to test all possible combinations of
 *       input parameters and return values.
 */
static void tfm_ipc_test_1001(struct test_result_t *ret)
{
    psa_framework_version_test(ret);
}

/**
 * \brief Retrieve the version of an RoT Service.
 */
static void tfm_ipc_test_1002(struct test_result_t *ret)
{
    psa_version_test(IPC_SERVICE_TEST_BASIC_SID, ret);
}

/**
 * \brief Call a Connection-based RoT Service.
 */
static void tfm_ipc_test_1004(struct test_result_t *ret)
{
    psa_handle_t handle;

    handle = psa_connect(IPC_SERVICE_TEST_BASIC_SID,
                         IPC_SERVICE_TEST_BASIC_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connection to service failed!\r\n");
        return;
    }

    request_rot_service_test(handle, ret);

    psa_close(handle);
}

/**
 * \brief Accessing a stateless service
 *
 * \note Accessing stateless service from a secure partition.
 */
static void tfm_ipc_test_1012(struct test_result_t *ret)
{
    request_rot_service_test(IPC_SERVICE_TEST_STATELESS_ROT_HANDLE, ret);
}
