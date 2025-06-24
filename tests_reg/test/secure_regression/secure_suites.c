/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "secure_suites.h"
#include "test_framework.h"

/* Service specific includes */
#ifdef TEST_S_PS
#include "ps_tests.h"
#endif
#ifdef TEST_S_ITS
#include "its_s_tests.h"
#endif
#ifdef TEST_S_ATTESTATION
#include "attest_s_tests.h"
#endif
#ifdef TEST_S_CRYPTO
#include "crypto_s_tests.h"
#endif
#ifdef TEST_S_FWU
#include "fwu_s_tests.h"
#endif
#ifdef TEST_S_PLATFORM
#include "platform_s_tests.h"
#endif
#ifdef TEST_S_IPC
#include "ipc_s_tests.h"
#endif
#ifdef TEST_S_SFN_BACKEND
#include "sfn_s_tests.h"
#endif
#if defined (TEST_S_FPU)
#include "fpu_s_tests.h"
#endif
#ifdef EXTRA_S_TEST_SUITE
#include "extra_s_tests.h"
#endif

static struct test_suite_t test_suites[] = {
#ifdef TEST_S_IPC
    /* Secure IPC test cases */
    {&register_testsuite_s_ipc_interface, 0, 0, 0},
#endif

#ifdef TEST_S_SFN_BACKEND
    /* Secure SFN backend test cases */
    {&register_testsuite_s_sfn_interface, 0, 0, 0},
#endif

#ifdef TEST_S_PS
    {&register_testsuite_s_psa_ps_interface, 0, 0, 0},
    {&register_testsuite_s_psa_ps_reliability, 0, 0, 0},
#ifdef PS_TEST_NV_COUNTERS
#if defined(PS_ROLLBACK_PROTECTION) && (PS_ROLLBACK_PROTECTION == 1)
    {&register_testsuite_s_rollback_protection, 0, 0, 0},
#endif /* PS_ROLLBACK_PROTECTION == 1 */
#endif /* PS_TEST_NV_COUNTERS */
#endif /* TEST_S_PS */

#ifdef TEST_S_ITS
    /* Secure ITS test cases */
    {&register_testsuite_s_psa_its_interface, 0, 0, 0},
    {&register_testsuite_s_psa_its_reliability, 0, 0, 0},
#endif

#ifdef TEST_S_CRYPTO
    /* Crypto test cases */
    {&register_testsuite_s_crypto_interface, 0, 0, 0},
#endif

#ifdef TEST_S_ATTESTATION
    /* Secure initial attestation service test cases */
    {&register_testsuite_s_attestation_interface, 0, 0, 0},
#endif

#ifdef TEST_S_PLATFORM
    /* Secure platform service test cases */
    {&register_testsuite_s_platform_interface, 0, 0, 0},
#endif

#ifdef TEST_S_FWU
    /* Secure Firmware Update test cases */
    {&register_testsuite_s_psa_fwu_interface, 0, 0, 0},
#endif

#ifdef TEST_S_FPU
    /* Secure FPU test cases */
    {&register_testsuite_s_fpu_interface, 0, 0, 0},
#endif

    /* Run extra tests as last test suite, this way platform
     * can execute some code after all tests are done
     */
#ifdef EXTRA_S_TEST_SUITE
    /* Secure extra test cases */
    {&register_testsuite_extra_s_interface, 0, 0, 0},
#endif

    /* End of test suites */
    {0, 0, 0, 0}
};

static void setup_integ_test(void)
{
    /* Left empty intentionally, currently implemented
     * test suites require no setup
     */
}

static void tear_down_integ_test(void)
{
    /* Left empty intentionally, currently implemented
     * test suites require no tear down
     */
}

enum test_suite_err_t s_reg_test_start(void)
{
    enum test_suite_err_t retval;

    setup_integ_test();
    retval = run_test("Secure", test_suites);
    tear_down_integ_test();
    return retval;
}
