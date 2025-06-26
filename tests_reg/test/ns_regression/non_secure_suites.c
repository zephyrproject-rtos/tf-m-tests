/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2022, Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "non_secure_suites.h"
#include "test_framework.h"

/* Service specific includes */
#ifdef TEST_NS_PS
#include "ps_ns_tests.h"
#endif
#ifdef TEST_NS_ITS
#include "its_ns_tests.h"
#endif
#ifdef TEST_NS_CRYPTO
#include "crypto_ns_tests.h"
#endif
#ifdef TEST_NS_FWU
#include "fwu_ns_tests.h"
#endif
#ifdef TEST_NS_ATTESTATION
#include "attest_ns_tests.h"
#endif
#ifdef TEST_NS_QCBOR
#include "qcbor_ns_tests.h"
#endif
#ifdef TEST_NS_T_COSE
#include "t_cose_ns_tests.h"
#endif
#ifdef TEST_NS_PLATFORM
#include "platform_ns_tests.h"
#endif
#ifdef TEST_NS_IPC
#include "ipc_ns_tests.h"
#else
#endif /* TEST_NS_IPC */
#ifdef TEST_NS_MULTI_CORE
#include "multi_core_ns_test.h"
#endif /* TEST_NS_MULTI_CORE */
#ifdef TEST_NS_MANAGE_NSID
#include "nsid_testsuite.h"
#endif /* TEST_NS_MANAGE_NSID */
#if defined(TEST_NS_SLIH_IRQ) || defined(TEST_NS_FLIH_IRQ)
#include "irq_testsuite.h"
#endif
#ifdef TEST_NS_SFN_BACKEND
#include "sfn_ns_tests.h"
#endif
#ifdef TEST_NS_FPU
#include "fpu_ns_tests.h"
#endif
#ifdef EXTRA_NS_TEST_SUITE
#include "extra_ns_tests.h"
#endif

static struct test_suite_t test_suites[] = {
    /* List test cases which are compliant with level 1 isolation */
#ifdef TEST_NS_IPC
    /* Non-secure IPC test cases */
    {&register_testsuite_ns_ipc_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_SFN_BACKEND
    {&register_testsuite_ns_sfn_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_PS
    {&register_testsuite_ns_psa_ps_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_ITS
    /* Non-secure ITS test cases */
    {&register_testsuite_ns_psa_its_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_CRYPTO
    /* Non-secure Crypto test cases */
    {&register_testsuite_ns_crypto_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_PLATFORM
    /* Non-secure platform service test cases */
    {&register_testsuite_ns_platform_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_ATTESTATION
    /* Non-secure initial attestation service test cases */
    {&register_testsuite_ns_attestation_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_QCBOR
    /* Non-secure QCBOR library test cases */
    {&register_testsuite_ns_qcbor, 0, 0, 0},
#endif

#ifdef TEST_NS_T_COSE
    /* Non-secure T_COSE library test cases */
    {&register_testsuite_ns_t_cose, 0, 0, 0},
#endif

#ifdef TEST_NS_FWU
    /* Non-secure Firmware Update test cases */
    {&register_testsuite_ns_psa_fwu_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_MULTI_CORE
    /* Multi-core topology test cases */
    {&register_testsuite_multi_core_ns_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_MANAGE_NSID
    {&register_testsuite_nsid_test, 0, 0, 0},
#endif /* TEST_NS_MANAGE_NSID */

#if defined(TEST_NS_SLIH_IRQ) || defined(TEST_NS_FLIH_IRQ)
    {&register_testsuite_irq_test, 0, 0, 0},
#endif

#ifdef TEST_NS_FPU
    {&register_testsuite_ns_fpu_interface, 0, 0, 0},
#endif

    /* Run extra tests as last test suite, this way platform
     * can execute some code after all tests are done
     */
#ifdef EXTRA_NS_TEST_SUITE
    /* Non-secure extra test cases */
    {&register_testsuite_extra_ns_interface, 0, 0, 0},
#endif

    /* End of test suites */
    {0, 0, 0, 0}
};

enum test_suite_err_t ns_reg_test_start(void)
{
    return run_test("Non-secure", test_suites);
}
