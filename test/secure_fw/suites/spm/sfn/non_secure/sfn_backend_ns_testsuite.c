/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include "sfn_ns_tests.h"
#include "psa/client.h"
#include "psa/framework_feature.h"
#include "test_framework_helpers.h"
#include "psa_manifest/sid.h"
#include "tfm_sfn_test_defs.h"
#if PSA_FRAMEWORK_HAS_MM_IOVEC
#include "mmiovec_test.h"
#endif

/* List of tests */
static void tfm_sfn_test_1001(struct test_result_t *ret);
static void tfm_sfn_test_1002(struct test_result_t *ret);
static void tfm_sfn_test_1003(struct test_result_t *ret);
static void tfm_sfn_test_1004(struct test_result_t *ret);
#if PSA_FRAMEWORK_HAS_MM_IOVEC
static void tfm_sfn_test_1005(struct test_result_t *ret);
static void tfm_sfn_test_1006(struct test_result_t *ret);
static void tfm_sfn_test_1007(struct test_result_t *ret);
#endif

static struct test_t sfn_veneers_tests[] = {
    {&tfm_sfn_test_1001, "TFM_NS_SFN_TEST_1001",
     "Get PSA framework version"},
    {&tfm_sfn_test_1002, "TFM_NS_SFN_TEST_1002",
     "Get version of an RoT Service"},
    {&tfm_sfn_test_1003, "TFM_NS_SFN_TEST_1003",
     "Request a connection-based RoT Service"},
    {&tfm_sfn_test_1004, "TFM_NS_SFN_TEST_1004",
     "Request a stateless RoT Service"},
#if PSA_FRAMEWORK_HAS_MM_IOVEC
    {&tfm_sfn_test_1005, "TFM_NS_SFN_TEST_1005",
     "Mapping input vectors and unmapping them. "},
    {&tfm_sfn_test_1006, "TFM_NS_SFN_TEST_1006",
     "Mapping output vectors and unmapping them. "},
    {&tfm_sfn_test_1007, "TFM_NS_SFN_TEST_1007",
     "Mapping output vectors and not unmapping them. "},
#endif
};

void register_testsuite_ns_sfn_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(sfn_veneers_tests) / sizeof(sfn_veneers_tests[0]));

    set_testsuite("SFN non-secure interface test (TFM_NS_SFN_TEST_1XXX)",
                  sfn_veneers_tests, list_size, p_test_suite);
}

/**
 * \brief Retrieve the version of the PSA Framework API.
 */
static void tfm_sfn_test_1001(struct test_result_t *ret)
{
    uint32_t version;

    version = psa_framework_version();
    if (version == PSA_FRAMEWORK_VERSION) {
        TEST_LOG("The version of the PSA Framework API is %d.\r\n", version);
    } else {
        TEST_FAIL("The version of the PSA Framework API is not valid!\r\n");
    }
}

/**
 * \brief Retrieve the version of an RoT Service.
 */
static void tfm_sfn_test_1002(struct test_result_t *ret)
{
    uint32_t version;

    version = psa_version(TFM_SFN1_SERVICE1_SID);
    if (version == PSA_VERSION_NONE) {
        TEST_FAIL("RoT Service is not implemented or caller is not authorized" \
                  " to access it!\r\n");
    } else {
        /* Valid version number */
        TEST_LOG("The service version is %d.\r\n", version);
        ret->val = TEST_PASSED;
    }
}

/**
 * \brief Request a connection-based RoT Service
 */
static void tfm_sfn_test_1003(struct test_result_t *ret)
{
    char in_str[SFN_SERVICE_BUFFER_LEN] = "It's a SFN service test.";
    uint32_t in_len = strlen(in_str) + 1;
    char out_str[SFN_SERVICE_BUFFER_LEN] = {'\0'};
    psa_invec invecs[] = {{in_str, in_len}};
    psa_outvec outvecs[] = {{out_str, sizeof(out_str)}};
    psa_handle_t handle;
    psa_status_t status;

    handle = psa_connect(TFM_SFN2_SERVICE1_SID, TFM_SFN2_SERVICE1_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connecting to a connection-based service fails.\r\n");
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, invecs, IOVEC_LEN(invecs),
                      outvecs, IOVEC_LEN(outvecs));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_call fails!\r\n");
        return;
    }

    /* Check the data in outvec is correct */
    if (memcmp(invecs[0].base, outvecs[0].base, in_len) == 0) {
        TEST_LOG("Request succeeds. Outvec is: %s\r\n", outvecs[0].base);
    } else {
        TEST_FAIL("Outvec data length is too long.\r\n");
    }

    psa_close(handle);
    ret->val = TEST_PASSED;
}

/**
 * \brief Request a stateless RoT Service
 */
static void tfm_sfn_test_1004(struct test_result_t *ret)
{
    uint32_t data = SFN_SERVICE_MAGIC;
    psa_handle_t handle;
    psa_status_t status;
    psa_invec invecs[] = {{&data, sizeof(data)}};

    /* Connecting to a stateless service should fail. */
    handle = psa_connect(TFM_SFN1_SERVICE1_SID, TFM_SFN1_SERVICE1_VERSION);
    if (PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connecting to a stateless service should not succeed.\r\n");
        return;
    }

    status = psa_call(TFM_SFN1_SERVICE1_HANDLE, PSA_IPC_CALL,
                      invecs, IOVEC_LEN(invecs), NULL, 0);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Calling a stateless service test fails.\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}

#if PSA_FRAMEWORK_HAS_MM_IOVEC
/**
 * \brief Mapping input vectors and unmapping them.
 *
 * \note Test psa_map_invec() and psa_unmap_invec() functionality.
 */
static void tfm_sfn_test_1005(struct test_result_t *ret)
{
    invec_map_unmap_test(ret, TFM_SFN1_SERVICE1_HANDLE);
}

/**
 * \brief Mapping output vectors and unmapping them.
 *
 * \note Test psa_map_outvec() and psa_unmap_outvec() functionality.
 */
static void tfm_sfn_test_1006(struct test_result_t *ret)
{
    outvec_map_unmap_test(ret, TFM_SFN1_SERVICE1_HANDLE);
}

/**
 * \brief Mapping output vectors and not unmapping them.
 *
 * \note RoT services map outvecs and does not unmap outvecs, the service caller
 *       should get a zero out length.
 */
static void tfm_sfn_test_1007(struct test_result_t *ret)
{
    outvec_map_only_test(ret, TFM_SFN1_SERVICE1_HANDLE);
}

#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */
