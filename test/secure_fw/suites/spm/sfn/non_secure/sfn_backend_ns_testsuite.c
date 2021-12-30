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
#include "client_api_tests.h"
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
    psa_framework_version_test(ret);
}

/**
 * \brief Retrieve the version of an RoT Service.
 */
static void tfm_sfn_test_1002(struct test_result_t *ret)
{
    psa_version_test(TFM_SFN1_SERVICE1_SID, ret);
}

/**
 * \brief Request a connection-based RoT Service
 */
static void tfm_sfn_test_1003(struct test_result_t *ret)
{
    psa_handle_t handle;

    handle = psa_connect(TFM_SFN2_SERVICE1_SID, TFM_SFN2_SERVICE1_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connecting to a connection-based service fails.\r\n");
        return;
    }

    request_rot_service_test(handle, ret);

    psa_close(handle);
}

/**
 * \brief Request a stateless RoT Service
 */
static void tfm_sfn_test_1004(struct test_result_t *ret)
{
    psa_handle_t handle;

    /* Connecting to a stateless service should fail. */
    handle = psa_connect(TFM_SFN1_SERVICE1_SID, TFM_SFN1_SERVICE1_VERSION);
    if (PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connecting to a stateless service should not succeed.\r\n");
        return;
    }

    request_rot_service_test(TFM_SFN1_SERVICE1_HANDLE, ret);
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
