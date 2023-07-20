*********
eRPC Host
*********

This host example is only tested on Linux machine with Musca-S1 and AN521 FVP.

Example: UART Transportation on Musca-S1
========================================

Build instructions on the target
--------------------------------

.. code-block:: bash

    cd <TF-M base folder>
    cmake -G"Unix Makefiles" -S . -B cmake_build -DTFM_PLATFORM=musca_s1 \
    -DCONFIG_TFM_ERPC_TEST_FRAMEWORK=ON -DTFM_PARTITION_CRYPTO=ON -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE=ON \
    -DTFM_SPM_LOG_LEVEL=TFM_SPM_LOG_LEVEL_SILENCE -DTFM_PARTITION_LOG_LEVEL=TFM_PARTITION_LOG_LEVEL_SILENCE \
    -DMCUBOOT_LOG_LEVEL=OFF ../
    cmake --build cmake_build/ -- install -j32

.. Note::
    All the logs must be disabled to avoid interferences between logs and eRPC transportation as
    they share the same UART device.
    Otherwise, the RPC test framework may not work.
    This might only happen on platforms using the same UART devices.

Build instructions on the host
------------------------------

.. code-block:: bash

    cd <TF-M tests base folder>/erpc/host_example
    cmake -S . -B build -G "Unix Makefiles" -DTFM_INSTALL_PATH=<TF-M install folder> -DERPC_REPO_PATH=<eRPC base folder> \
    -DERPC_TRANSPORT=UART -DPORT_NAME="/dev/ttyACM0"
    cmake --build  build/

Run instructions
----------------

- Flash the image to the device and boot it up.
- Start the host example

  .. code-block:: bash

      cd <TF-M tests base folder>/erpc/host_example
      ./build/erpc_main

Example: TCP Transportation on AN521 FVP
========================================

Build instructions on the target
--------------------------------

.. code-block:: bash

    cd <TF-M base folder>
    cmake -G"Unix Makefiles" -S . -B cmake_build -DTFM_PLATFORM=an521 \
    -DCONFIG_TFM_ERPC_TEST_FRAMEWORK=ON -DTFM_PARTITION_CRYPTO=ON -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE=ON ../
    cmake --build cmake_build/ -- install -j32

Build instructions on the host
------------------------------

.. code-block:: bash

    cd <TF-M tests base folder>/erpc/host_example
    cmake -S . -B build -G "Unix Makefiles" -DTFM_INSTALL_PATH=<TF-M install folder> -DERPC_REPO_PATH=<eRPC base folder> \
    -DERPC_TRANSPORT=TCP -DERPC_HOST="0.0.0.0" -DERPC_PORT=5001
    cmake --build  build/

Run instructions
----------------

Start the AN521 FVP:

.. code-block:: bash

    <DS_PATH>/sw/models/bin/FVP_MPS2_AEMv8M  \
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
    --parameter fvp_mps2.UART0.out_file=/dev/stdout \
    --parameter fvp_mps2.UART0.unbuffered_output=1 \
    --parameter fvp_mps2.telnetterminal1.mode=raw \
    --parameter fvp_mps2.UART1.unbuffered_output=1 \
    --parameter fvp_mps2.mps2_visualisation.disable-visualisation=1 \
    --application cpu0=<APPLICATION> \
    --data cpu0=<DATA>@0x10080000 \
    -M 1

Start the host example

.. code-block:: bash

    cd <TF-M tests base folder>/erpc/host_example
    ./build/erpc_main

--------------

*Copyright (c) 2023, Arm Limited. All rights reserved.*
