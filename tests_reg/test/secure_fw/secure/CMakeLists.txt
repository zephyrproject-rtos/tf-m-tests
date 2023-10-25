#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

# Add Secure test suites.
add_subdirectory(../suites/attestation/secure   ${CMAKE_CURRENT_BINARY_DIR}/suites/attestation)
add_subdirectory(../suites/crypto/secure        ${CMAKE_CURRENT_BINARY_DIR}/suites/crypto)
add_subdirectory(../suites/extra/secure         ${CMAKE_CURRENT_BINARY_DIR}/suites/extra)
add_subdirectory(../suites/its/secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/its)
add_subdirectory(../suites/ps/secure            ${CMAKE_CURRENT_BINARY_DIR}/suites/ps)
add_subdirectory(../suites/platform/secure      ${CMAKE_CURRENT_BINARY_DIR}/suites/platform)
add_subdirectory(../suites/fwu/secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/fwu)
add_subdirectory(../suites/fpu/secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/fpu)
add_subdirectory(../suites/spm/secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/spm)
