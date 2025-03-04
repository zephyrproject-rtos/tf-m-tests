###################################
Run TF-M regression tests with eRPC
###################################

This document aims to show the steps of building and running the NS regression
test suites based on TF-M's eRPC framework and to serve as an example
which can be followed for other tests and basically for any application
that wants to interface with TF-M through the PSA client APIs using eRPC.

The description assumes that the basic steps and requirements [1]_ of building
the TF-M regression test code are known.

***************************
eRPC specific build options
***************************

When using the eRPC framework it is required to provide a few additional
parameters to specify which kind of transportation should be used for a given
target. These parameters depending on the transport of choice are listed below
and should be passed as CMake command line parameters.

+----------------+--------------------------------------------------------------------------+
| Parameter      | Description                                                              |
+================+==========================================================================+
| ERPC_TRANSPORT | Selected method of transportation. It can be either ``TCP`` or ``UART``. |
+----------------+--------------------------------------------------------------------------+
| ERPC_HOST      | Hostname/IP address of eRPC server to connect to (for TCP only).         |
+----------------+--------------------------------------------------------------------------+
| ERPC_PORT      | Port number of eRPC server to connect to (for TCP only).                 |
+----------------+--------------------------------------------------------------------------+
| PORT_NAME      | Serial port to use for communication with eRPC server (for UART only).   |
+----------------+--------------------------------------------------------------------------+

As it was already mentioned in the
:doc:`TF-M eRPC Test Framework <tfm_test_suites_addition>` documentation,
it is recommended to assign a separate UART device to the eRPC
communication to avoid collision with other data (e.g. log messages) being sent
on UART. In the simplest configuration if there is only one serial device
available on the target (due to any limitation) all types of log messages from
the device must be disabled. When the build type is set to either
``MinSizeRel`` (default) or ``Release`` all logging in TF-M
(BLx, SPM, Secure Partitions) is disabled automatically. Otherwise, one must
disable logging manually by setting the following build options
(if available on the given target):

+-------------------------+-------------------------------------+
| Parameter               | Value                               |
+=========================+=====================================+
| TFM_BL1_LOG_LEVEL       | ``LOG_LEVEL_NONE``                  |
+-------------------------+-------------------------------------+
| MCUBOOT_LOG_LEVEL       | ``OFF``                             |
+-------------------------+-------------------------------------+
| TFM_SPM_LOG_LEVEL       | ``TFM_SPM_LOG_LEVEL_SILENCE``       |
+-------------------------+-------------------------------------+
| TFM_PARTITION_LOG_LEVEL | ``LOG_LEVEL_NONE``                  |
+-------------------------+-------------------------------------+

Execute tests on TC3 FVP with TCP Transport
===========================================

On TC3 we are using the same UART device for eRPC communication as for logging.
In this case we are setting the build type to ``Release`` and therefore we do
not need to manually disable any logs.

Build instructions for TC3
--------------------------

Build TF-M (target SPE image):

.. code-block:: bash

    cd <TF-M tests base folder>
    cmake -S tests_reg/spe -B <TF-M build dir> \
        -DTFM_PLATFORM=arm/rse/tc/tc3 \
        -DCONFIG_TFM_SOURCE_PATH=<TF-M source dir absolute path> \
        -DCMAKE_BUILD_TYPE=Release \
        -DTFM_TOOLCHAIN_FILE=<TF-M source dir absolute path>/toolchain_GNUARM.cmake \
        -DMCUBOOT_IMAGE_NUMBER=2 \
        -DRSE_LOAD_NS_IMAGE=ON \
        -DTEST_NS=ON
    cmake --build <TF-M build dir> -- install

Build eRPC server application (target NSPE image):

.. code-block:: bash

    cd <TF-M tests base folder>
    cmake -S erpc/server/app -B <NS build dir> \
        -DCONFIG_SPE_PATH=<TF-M build dir>/api_ns \
        -DCMAKE_BUILD_TYPE=Release
    cmake --build <NS build dir>

Build test application and eRPC client (host image):

.. code-block:: bash

    cd <TF-M tests base folder>/erpc/tfm_reg_tests
    cmake -S . -B <Test app build dir> \
        -DCONFIG_SPE_PATH=<TF-M build dir>/api_ns \
        -DERPC_REPO_PATH=<NS build dir>/lib/ext/erpc-src \
        -DERPC_TRANSPORT=TCP \
        -DERPC_HOST="0.0.0.0" \
        -DERPC_PORT=5005
    cmake --build <Test app build dir>

Run the code on the TC3 FVP model
---------------------------------

The basic steps of creating the required images (e.g ROM and flash images)
for the Total Compute TC3 platform are covered in the `RSE documentation
<https://trustedfirmware-m.readthedocs.io/en/latest/platform/arm/rse/readme.html>`_.
To run the tests, the latest available TC3 FVP (Fixed Virtual Platform) model
can be downloaded from the `Arm Developer Hub
<https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms/Total%20Compute%20FVPs>`_.

.. code-block:: bash

    <PATH to FVP model folder>FVP_TC3 \
        -C css.terminal_uart_ap.start_port=5000 \
        -C css.terminal_uart1_ap.start_port=5001 \
        -C css.sms.scp.terminal_uart.start_port=5002 \
        -C css.sms.rse_terminal_uart.start_port=5003 \
        -C soc.terminal_s0.start_port=5004 \
        -C soc.terminal_s1.start_port=5005 \
        -C soc.terminal_s1.start_telnet=0 \
        -C soc.terminal_s1.quiet=1 \
        -C soc.terminal_s1.mode=raw \
        -C soc.pl011_uart1.unbuffered_output=1 \
        -C soc.pl011_uart1.enable_dc4=0 \
        -C displayController=2 \
        -C css.sms.rse.sic.SIC_AUTH_ENABLE=1 \
        -C css.sms.rse.sic.SIC_DECRYPT_ENABLE=1 \
        -C css.sms.rse.VMADDRWIDTH=16 \
        -C css.sms.rse.intchecker.ICBC_RESET_VALUE=0x0000011B \
        -C css.sms.rse.rom.raw_image=<rse_rom.bin> \
        -C board.flashloader0.fname=<host_flash_fip.bin> \
        --data css.sms.rse.sram0=<encrypted_cm_provisioning_bundle_0.bin>@0x400 \
        --data css.sms.rse.sram1=<encrypted_dm_provisioning_bundle_0.bin>@0x0

Execute tests on AN521 FVP with TCP Transport
=============================================

On this platform several UART devices are available, therefore we can assign
an unused one exclusively to the eRPC communication while we can keep all
logging enabled.

Build instructions for AN521
----------------------------

Build TF-M (device SPE image):

.. code-block:: bash

    cd <TF-M tests base folder>
    cmake -S tests_reg/spe -B <TF-M build dir> \
        -DTFM_PLATFORM=arm/mps2/an521 \
        -DCONFIG_TFM_SOURCE_PATH=<TF-M source dir absolute path> \
        -DCMAKE_BUILD_TYPE=Debug \
        -DTFM_TOOLCHAIN_FILE=<TF-M source dir absolute path>/toolchain_GNUARM.cmake \
        -DTEST_NS=ON
    cmake --build <TF-M build dir> -- install

Build eRPC server application (device NSPE image):

.. code-block:: bash

    cd <TF-M tests base folder>
    cmake -S erpc/server/app -B <NS build dir> \
        -DCONFIG_SPE_PATH=<TF-M build dir>/api_ns \
        -DCMAKE_BUILD_TYPE=Debug
    cmake --build <NS build dir>


Build test application and eRPC client (host image):

.. code-block:: bash

    cd <TF-M tests base folder>/erpc/tfm_reg_tests
    cmake -S . -B <Test app build dir> \
        -DCONFIG_SPE_PATH=<TF-M build dir>/api_ns \
        -DERPC_REPO_PATH=<NS build dir>/lib/ext/erpc-src \
        -DERPC_TRANSPORT=TCP \
        -DERPC_HOST="0.0.0.0" \
        -DERPC_PORT=5001
    cmake --build <Test app build dir>

Run the code on the AN521 FVP model
-----------------------------------

To run the test application we are using the FVP_MPS2_AEMv8M model provided
by `Arm Development Studio`_ or the FVP can be downloaded from the
`Developer Hub <https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms/Arm%20Architecture%20FVPs>`_.

.. code-block:: bash

    <PATH to FVP model folder>/FVP_MPS2_AEMv8M  \
        --parameter fvp_mps2.platform_type=2 \
        --parameter cpu0.baseline=0 \
        --parameter cpu0.INITVTOR_S=0x10000000 \
        --parameter cpu0.semihosting-enable=0 \
        --parameter fvp_mps2.DISABLE_GATING=0 \
        --parameter fvp_mps2.telnetterminal0.start_telnet=1 \
        --parameter fvp_mps2.telnetterminal1.start_telnet=0 \
        --parameter fvp_mps2.telnetterminal2.start_telnet=0 \
        --parameter fvp_mps2.telnetterminal0.quiet=0 \
        --parameter fvp_mps2.telnetterminal1.quiet=1 \
        --parameter fvp_mps2.telnetterminal2.quiet=1 \
        --parameter fvp_mps2.telnetterminal0.start_port=5000 \
        --parameter fvp_mps2.telnetterminal1.start_port=5001 \
        --parameter fvp_mps2.telnetterminal1.mode=raw \
        --parameter fvp_mps2.UART1.unbuffered_output=1 \
        --application cpu0=<TF-M build dir>/bin/bl2.axf \
        --data cpu0=<NS build dir>/tfm_s_ns_signed.bin@0x10080000 \
        -M 1

References
----------

.. [1] :doc:`Building TF-M Tests <TF-M:building/tests_build_instruction>`

.. _Arm Development Studio: https://developer.arm.com/tools-and-software/embedded/arm-development-studio

--------------

 *SPDX-License-Identifier: BSD-3-Clause*
 *SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors*
