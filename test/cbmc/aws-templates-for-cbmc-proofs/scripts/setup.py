#!/usr/bin/env python3

# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

"""Set up the CBMC proof instrastructure."""

import logging
import os

import util

def create_makefile_template_defines(source_root, proof_root):
    """Create Makefile-template-defines in the proof root."""

    makefile = os.path.join(proof_root, "Makefile-template-defines")
    if os.path.exists(makefile):
        logging.warning("Overwriting %s", makefile)

    with open(makefile, "w") as fileobj:
        print("SRCDIR ?= $(abspath $(PROOF_ROOT)/{})"
              .format(os.path.relpath(source_root, proof_root)),
              file=fileobj)

def main():
    """Set up the CBMC proof infrastructure."""

    logging.basicConfig(format='%(levelname)s: %(message)s')

    source_root = util.read_source_root()
    cbmc_root = os.path.abspath('.')
    proof_root = util.proof_root(cbmc_root)

    util.copy_repository_templates(cbmc_root)
    create_makefile_template_defines(source_root, proof_root)

if __name__ == "__main__":
    main()
