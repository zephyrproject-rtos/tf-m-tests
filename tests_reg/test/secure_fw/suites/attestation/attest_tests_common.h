/*
 * SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ATTESTATION_TESTS_COMMON_H__
#define __ATTESTATION_TESTS_COMMON_H__

#include "psa/initial_attestation.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \def TEST_TOKEN_SIZE
 *
 * \brief Size of token buffer in bytes.
 */
#define TEST_TOKEN_SIZE         (PSA_INITIAL_ATTEST_MAX_TOKEN_SIZE)

/*!
 * \def TEST_CHALLENGE_OBJ_SIZE
 *
 * \brief Size of challenge object in bytes used for test.
 */
#define TEST_CHALLENGE_OBJ_SIZE (PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64)

/*!
 * \def INVALID_CHALLENGE_OBJECT_SIZE
 *
 * \brief Size of challenge object that is invalid.
 */
#define INVALID_CHALLENGE_OBJECT_SIZE (PSA_INITIAL_ATTEST_CHALLENGE_SIZE_32 + 1)

#ifdef __cplusplus
}
#endif

#endif /* __ATTESTATION_TESTS_COMMON_H__ */
