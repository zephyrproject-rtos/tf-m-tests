#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

# Add test suites.
# Secure test suite library targets shall already be added in secure_tests.cmake
add_subdirectory(attestation)
add_subdirectory(crypto)
add_subdirectory(extra)
add_subdirectory(its)
add_subdirectory(ps)
add_subdirectory(qcbor)
add_subdirectory(t_cose)
add_subdirectory(platform)
add_subdirectory(fwu)
add_subdirectory(multi_core/non_secure)
add_subdirectory(nsid)
add_subdirectory(fpu)
add_subdirectory(spm)
