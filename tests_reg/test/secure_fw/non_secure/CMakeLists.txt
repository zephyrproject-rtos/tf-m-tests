#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

# Add NS test suites.
add_subdirectory(../suites/attestation/non_secure   ${CMAKE_CURRENT_BINARY_DIR}/suites/attestation)
add_subdirectory(../suites/crypto/non_secure        ${CMAKE_CURRENT_BINARY_DIR}/suites/crypto)
add_subdirectory(../suites/extra/non_secure         ${CMAKE_CURRENT_BINARY_DIR}/suites/extra)
add_subdirectory(../suites/its/non_secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/its)
add_subdirectory(../suites/ps/non_secure            ${CMAKE_CURRENT_BINARY_DIR}/suites/ps)
add_subdirectory(../suites/qcbor/non_secure         ${CMAKE_CURRENT_BINARY_DIR}/suites/qcbor)
add_subdirectory(../suites/t_cose/non_secure        ${CMAKE_CURRENT_BINARY_DIR}/suites/t_cose)
add_subdirectory(../suites/platform/non_secure      ${CMAKE_CURRENT_BINARY_DIR}/suites/platform)
add_subdirectory(../suites/fwu/non_secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/fwu)
add_subdirectory(../suites/multi_core/non_secure    ${CMAKE_CURRENT_BINARY_DIR}/suites/multi_core)
add_subdirectory(../suites/nsid/non_secure          ${CMAKE_CURRENT_BINARY_DIR}/suites/nsid)
add_subdirectory(../suites/fpu/non_secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/fpu)
add_subdirectory(../suites/spm/non_secure           ${CMAKE_CURRENT_BINARY_DIR}/suites/spm)
