/*
 * attest_symmetric_iat_decode.c
 *
 * Copyright (c) 2019, Laurence Lundblade.
 * Copyright (c) 2020-2025, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "attest_token_decode.h"
#include "attest.h"
#include "psa/crypto.h"
#include "t_cose/q_useful_buf.h"
#include "qcbor_util.h"
#include "t_cose/t_cose_common.h"
#include "t_cose/t_cose_mac_validate.h"
#include "tfm_crypto_defs.h"

/* Only support HMAC as MAC algorithm in COSE_Mac0 so far */
#define SYMMETRIC_IAK_MAX_SIZE        PSA_MAC_MAX_SIZE

#if DOMAIN_NS == 1U
/*
 * Public function. See attest_token_decode.h
 * It is not allowed to let NS side fetch the symmetric IAK and perform the MAC
 * validation.
 */
enum attest_token_err_t
attest_token_decode_validate_token(struct attest_token_decode_context *me,
                                   struct q_useful_buf_c               token)
{
    enum t_cose_err_t              t_cose_error;
    enum attest_token_err_t        return_value;
    /* Decode only without authentication tag validation */
    int32_t                        t_cose_options = T_COSE_OPT_DECODE_ONLY;
    struct t_cose_mac_validate_ctx validate_ctx;
    struct t_cose_key              attest_key;

    t_cose_mac_validate_init(&validate_ctx, t_cose_options);

    /* Initialising key with invalid identifier; however with the
     * T_COSE_OPT_DECODE_ONLY option the validation step will be skipped
     * and the key won't be used.
     */
    attest_key.key.handle = (uint64_t)PSA_KEY_ID_NULL;
    t_cose_mac_set_validate_key(&validate_ctx, attest_key);

    t_cose_error = t_cose_mac_validate(&validate_ctx,
                                       token,         /* COSE to validate */
                                       NULL_Q_USEFUL_BUF_C,
                                       &me->payload,  /* Payload from token */
                                       NULL);

    return_value = map_t_cose_errors(t_cose_error);
    me->last_error = return_value;

    return return_value;
}

#else /* DOMAIN_NS == 1U */

/*
 * Public function. See attest_token_decode.h
 * Decode the received COSE_Mac0 structure and validate the tag. Authentication
 * tag validation in tests is for debug purpose only. The symmetric Initial
 * Attestation key (IAK) should not be able to be used by anything other than
 * the Attestation partition in real products.
 */
enum attest_token_err_t
attest_token_decode_validate_token(struct attest_token_decode_context *me,
                                   struct q_useful_buf_c               token)
{
    enum t_cose_err_t               t_cose_error;
    enum attest_token_err_t         return_value;
    int32_t                         t_cose_options = 0;
    struct t_cose_mac_validate_ctx  validate_ctx;
    struct t_cose_key               attest_key;
    psa_key_handle_t                key_handle = TFM_BUILTIN_KEY_ID_IAK;

    if (me->options & TOKEN_OPT_SHORT_CIRCUIT_SIGN) {
        t_cose_options |= T_COSE_OPT_ALLOW_SHORT_CIRCUIT;
    }

    t_cose_mac_validate_init(&validate_ctx, t_cose_options);

    attest_key.key.handle = (uint64_t)key_handle;
    t_cose_mac_set_validate_key(&validate_ctx, attest_key);

    t_cose_error = t_cose_mac_validate(&validate_ctx,
                                       token,         /* COSE to validate */
                                       NULL_Q_USEFUL_BUF_C,
                                       &me->payload,  /* Payload from token */
                                       NULL);

    return_value = map_t_cose_errors(t_cose_error);
    me->last_error = return_value;

    return return_value;
}
#endif /* DOMAIN_NS == 1U */
