/*
 * attest_token_decode.c
 *
 * Copyright (c) 2019, Laurence Lundblade.
 * Copyright (c) 2020-2025, Arm Limited.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "attest_token_decode.h"
#include "t_cose/t_cose_sign1_verify.h"
#include "t_cose/q_useful_buf.h"
#include "qcbor_util.h"
#include "attest.h"
#include "tfm_crypto_defs.h"

/**
 * \file attest_token_decode.c
 *
 * \brief Attestation token decoder for COSE_Sign1.
 */

/*
 * Public function. See attest_token_decode.h
 */
enum attest_token_err_t
attest_token_decode_validate_token(struct attest_token_decode_context *me,
                                   struct q_useful_buf_c token)
{
    enum t_cose_err_t              t_cose_error;
    enum attest_token_err_t        return_value;
    struct t_cose_sign1_verify_ctx verify_ctx;
    struct t_cose_key              attest_key;

    /* Run the signature verification */

    t_cose_sign1_verify_init(&verify_ctx, 0);

    attest_key.key.handle = TFM_BUILTIN_KEY_ID_IAK;

    t_cose_sign1_set_verification_key(&verify_ctx, attest_key);

    t_cose_error =  t_cose_sign1_verify(&verify_ctx,
                                        token, /* COSE to verify */
                                        &me->payload, /* Payload from token */
                                        NULL); /* Don't return parameters */

    return_value = map_t_cose_errors(t_cose_error);
    me->last_error = return_value;

    return return_value;
}
