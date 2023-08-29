/*
 * Copyright (c) 2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This is a stub functoins for TF-M shim layer of TZ APIs
 *
 */

#include "tz_context.h"
/*
 * TF-M shim layer of the CMSIS TZ RTOS thread context management API
 */

/*
 * Initialize token-nsid map table in tfm nsid manager
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_InitContextSystem_S(void)
{
    return 1U;    /* Success */
}

/*
 * Allocate context memory from Secure side
 * Param: TZ_ModuleId_t (NSID if TFM_NS_MANAGE_NSID is enabled)
 * Return token if TFM_NS_MANAGE_NSID is enabled
 * Return 0 if no memory available or internal error
 */
TZ_MemoryId_t TZ_AllocModuleContext_S(TZ_ModuleId_t module)
{
    return 1U;    /* Success */
}

/*
 * Free context memory that was previously allocated with TZ_AllocModuleContext_S
 * Param: TZ_MemoryId_t (token if TFM_NS_MANAGE_NSID is enabled)
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_FreeModuleContext_S(TZ_MemoryId_t id)
{
    return 1U;    /* Success */
}

/*
 * Load secure context (called on RTOS thread context switch)
 * Param: TZ_MemoryId_t (token if TFM_NS_MANAGE_NSID is enabled)
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_LoadContext_S(TZ_MemoryId_t id)
{
    return 1U;    /* Success */
}

/*
 * Store secure context (called on RTOS thread context switch)
 * Param: TZ_MemoryId_t (token if TFM_NS_MANAGE_NSID is enabled)
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_StoreContext_S(TZ_MemoryId_t id)
{
    return 1U;    /* Success */
}
