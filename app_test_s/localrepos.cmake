#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

message(WARNING "Local repositries are used for MBEDCRYPTO_PATH, MCUBOOT_PATH, QCBOR_PATH")

set(MBEDCRYPTO_PATH    /home/antkom01/x-temp/repos/mbedcrypto-src CACHE PATH "" FORCE)
set(MCUBOOT_PATH       /home/antkom01/x-temp/repos/mcuboot-src    CACHE PATH "" FORCE)
set(QCBOR_PATH         /home/antkom01/x-temp/repos/qcbor-src      CACHE PATH "" FORCE)
