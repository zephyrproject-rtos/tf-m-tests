#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# ----------------------------------------------------------------------------
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
# ----------------------------------------------------------------------------

"""Wycheproof test suite parser script.

This module implements a parser for ECDSA test schemes for signature
verification as described by the `Project Wycheproof test suite`_.

Example:
    Run the script with the `-h` option to get more information about the
    available options for parsing::

        $ python wp_ec_parser.py -h

The output of the script is a binary file which can then be imported in
the .data section at build time using `.incbin` in volatile assembly or
modifying the linker script, using `fwrite()` at runtime if available,
or incorporated in code by running `xxd -i` to generate code suitable to
be consumed from C or C++ code.

Attributes:
    def_f_in_name (str): Predefined name for the test JSON file to process. It
        maps to the SECP384R1 SHA-384 test suite in the Project Wycheproof repo

    def_f_out_name (str): Predefined name for the output, it just appends the
        `.bin` suffix to the default input file name which is
        `ecdsa_secp384r1_sha384_test.json`

    raw_repo_name (str): GitHub address (raw) for the Project Wycheproof repo

    remote_base (str): Location on the remote address where to find the test
        vectors

    remote_name (str): Actual file that it is going to be fetched with the
        option `--get`

.. _Project Wycheproof test suite:
   https://github.com/C2SP/wycheproof

"""

import argparse
import os
import struct
import sys
import urllib.request
from pathlib import Path

def_f_in_name = 'ecdsa_secp384r1_sha384_test.json'
def_f_out_name = def_f_in_name + '.bin'
raw_repo_name = 'https://raw.githubusercontent.com/C2SP/wycheproof'
remote_base = raw_repo_name + '/refs/heads/master/testvectors/'
remote_name = remote_base + def_f_in_name


def parse_wp_ecdsa(f_in_name=def_f_in_name, f_out_name=def_f_out_name):
    """Parses a JSON file for ECDSA verification tests and creates a binary

       Parses a JSON file describing a test scheme for ECDSA
       verification as specified in the Wycheproof test suite and
       creates a binary blob which can then be imported and run in
       the TF-M Crypto regression tests on the NS side

    Args:
        f_in_name (str, optional): Input file. Defaults to def_f_in_name.
        f_out_name (str, optional): Output file. Defaults to def_f_out_name.
    """
    check_key = True

    try:
        Path(f_in_name).resolve(strict=True)
    except FileNotFoundError:
        sys.exit('File {} does not exist'.format(f_in_name))

    with open(f_in_name, 'r') as f, open(f_out_name, 'wb') as g:

        grep_list = ['\"uncompressed\" :',
                     '\"sig\" :',
                     '\"msg\" :',
                     '\"result\" :']
        PSA_ERROR_INVALID_SIGNATURE = -149

        for line in f.readlines():
            for word in grep_list:
                if word in line:
                    el = line.replace('\n', '') \
                             .replace(' ', '')  \
                             .replace(',', '')  \
                             .replace('"', '')  \
                             .split(':')

                    if el[0] in ('sig', 'msg', 'uncompressed'):
                        s = (struct.pack('<I', len(bytes.fromhex(el[1]))),
                             bytes.fromhex(el[1]))
                        if (check_key is True and el[0] != 'uncompressed'):
                            g.write(b'\x00\x00\x00\x00')
                        check_key = False

                        for i in s:
                            g.write(i)
                    else:
                        result = PSA_ERROR_INVALID_SIGNATURE \
                            if el[1] == 'invalid' else 0
                        g.write(struct.pack('<i', result))
                        check_key = True

    print("Written {} bytes of data".format(os.path.getsize(f_out_name)))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description='Parser for ECDSA verification JSON based \
                     test case from Project Wycheproof')

    parser.add_argument(
        "test_dir", nargs='?', const=False,
        help="local test directory to look for the default test vector, \
              i.e. when --in is not set")
    parser.add_argument(
        "--get", nargs='?', const=True, metavar='filename',
        help="get the test vector from GitHub. If no filename \
              is specified, the default P384 test vector is fetched")
    parser.add_argument(
        "--in", metavar='filename', dest='cmd_f_in_name',
        help="name of the input json file to parse")
    parser.add_argument(
        "--out", metavar='filename', dest='cmd_f_out_name',
        help="name of the parsed output binary file")

    args = parser.parse_args()

    if args.get:

        if args.get is not True:
            if Path(args.get).suffix != '.json':
                sys.exit(
                    'Requested {} which is not a json file'.format(args.get))
            remote_name = remote_base + args.get
            print('Getting {} from {}'.format(args.get, remote_base))

        with urllib.request.urlopen(remote_name, timeout=2) as remote, \
             open('tmp.json', 'wb') as temp:
            data = remote.read()
            temp.write(data)

        parsed_remote = urllib.parse.urlparse(remote_name)
        f_in = os.path.basename(parsed_remote.path)

        parse_wp_ecdsa(f_in_name='tmp.json', f_out_name=f_in + '.bin')
        os.remove('tmp.json')

    elif args.cmd_f_in_name and not args.cmd_f_out_name:

        parse_wp_ecdsa(
            f_in_name=args.cmd_f_in_name,
            f_out_name=args.cmd_f_in_name + '.bin')

    elif not args.cmd_f_in_name and args.cmd_f_out_name:

        parse_wp_ecdsa(f_out_name=args.cmd_f_out_name)

    elif args.cmd_f_in_name and args.cmd_f_out_name:

        parse_wp_ecdsa(args.cmd_f_in_name, args.cmd_f_out_name)

    else:

        if args.test_dir:
            def_f_in_name = Path(args.test_dir, def_f_in_name)
            print("Looking for {}".format(def_f_in_name))
            parse_wp_ecdsa(f_in_name=def_f_in_name)
        else:
            parse_wp_ecdsa()
