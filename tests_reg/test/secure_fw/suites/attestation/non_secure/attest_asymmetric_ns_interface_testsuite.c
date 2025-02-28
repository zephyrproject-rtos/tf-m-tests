/*
 * SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <assert.h>
#include "attest_ns_tests.h"
#include "psa/initial_attestation.h"
#include "attest.h"
#include "../attest_tests_common.h"
#include "../attest_token_test_values.h"
#include "../attest_token_test.h"


/* Define test suite for attestation service tests */
/* List of tests */
static void tfm_attest_test_1001(struct test_result_t *ret);
static void tfm_attest_test_1002(struct test_result_t *ret);

static struct test_t attestation_interface_tests[] = {
    {&tfm_attest_test_1001, "TFM_NS_ATTEST_TEST_1001",
     "ECDSA signature test of attest token"},
    {&tfm_attest_test_1002, "TFM_NS_ATTEST_TEST_1002",
     "Negative test cases for initial attestation service"},
};

/*
 * Buffer for attestation token
 *
 * Construct a buffer with enough capacity to prevent overwrite and data
 * corruption in case buffer size check fails and the token is incorrectly
 * generated.
 */
static uint8_t token_buffer[TEST_TOKEN_SIZE];

void
register_testsuite_ns_attestation_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(attestation_interface_tests) /
                 sizeof(attestation_interface_tests[0]));

    set_testsuite("Initial Attestation Service non-secure interface tests"
                  "(TFM_NS_ATTEST_TEST_1XXX)",
                  attestation_interface_tests, list_size, p_test_suite);
}

/*!
 * \brief Get an IAT with proper ECDSA signature. Parse the token, validate
 *        presence of claims and compare them against expected values in
 *        token_test_values.h
 *
 * ECDSA signing is currently not supported in TF_M.
 *
 * More info in token_test.h
 */
static void tfm_attest_test_1001(struct test_result_t *ret)
{
    int32_t err;

    err = decode_test_normal_sig();
    if (err != 0) {
        TEST_LOG("decode_test_normal_sig() returned: %d\r\n", err);
        TEST_FAIL("Attest token decode_test_normal_sig() has failed");
        return;
    }

    ret->val = TEST_PASSED;
}

/*!
 * \brief Negative tests for initial attestation service
 *
 *    - Calling initial attestation service with bigger challenge object than
 *      allowed.
 *    - Calling initial attestation service with smaller buffer size than the
 *      expected size of the token.
 */
static void tfm_attest_test_1002(struct test_result_t *ret)
{
    const uint8_t challenge_buffer[TEST_CHALLENGE_OBJ_SIZE] = {0};
    size_t token_size;
    psa_status_t err;

    /* Call with with bigger challenge object than allowed.
     *
     * Only discrete sizes are accepted which are defined by the
     * PSA_INITIAL_ATTEST_CHALLENGE_SIZE_<x> constants.
     */
    err = psa_initial_attest_get_token(challenge_buffer,
                                       INVALID_CHALLENGE_OBJECT_SIZE,
                                       token_buffer,
                                       sizeof(token_buffer),
                                       &token_size);

    if (err != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Attestation should fail with too big challenge object");
        return;
    }

    /* First get the size of the expected token */
    err = psa_initial_attest_get_token_size(sizeof(challenge_buffer),
                                            &token_size);
    if (err != PSA_SUCCESS) {
        TEST_FAIL("Attest test tfm_attest_test_1002() has failed");
        return;
    }

    assert(sizeof(token_buffer) >= token_size);

    /* Call with smaller buffer size than size of the expected token */
    err = psa_initial_attest_get_token(challenge_buffer,
                                       sizeof(challenge_buffer),
                                       token_buffer,
                                       (token_size - 1),
                                       &token_size);

    if (err != PSA_ERROR_BUFFER_TOO_SMALL) {
        TEST_FAIL("Attestation should fail with too small token buffer");
        return;
    }

    ret->val = TEST_PASSED;
}
