#-------------------------------------------------------------------------------
# SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(CONFIG_TFM_ERPC_TEST_FRAMEWORK)
    add_subdirectory(erpc)
endif()

if(TFM_PARTITION_INITIAL_ATTESTATION)
    add_subdirectory(qcbor)
    add_subdirectory(t_cose)
endif()
add_subdirectory(CMSIS)
