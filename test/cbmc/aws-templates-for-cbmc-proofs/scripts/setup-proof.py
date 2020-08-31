#!/usr/bin/env python3

# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

"""Set up a CBMC proof."""

import logging
import os
import shutil

import util

def proof_template_filenames():
    directory = os.path.join(util.templates_root(), util.PROOF_TEMPLATES)
    return os.listdir(directory)

def read_proof_template(filename):
    directory = os.path.join(util.templates_root(), util.PROOF_TEMPLATES)
    with open(os.path.join(directory, filename)) as data:
        return data.read().splitlines()

def write_proof_template(lines, filename, directory):
    with open(os.path.join(directory, filename), "w") as data:
        data.write('\n'.join(lines))

def rename_proof_harness(function, directory):
    shutil.move(os.path.join(directory, "FUNCTION_harness.c"),
                os.path.join(directory, "{}_harness.c".format(function)))

def patch_function_name(lines, function):
    return [line.replace("<__FUNCTION_NAME__>", function) for line in lines]

def patch_path_to_makefile(lines, proof_root, proof_dir):
    path = os.path.relpath(proof_root, proof_dir)
    return [line.replace("<__PATH_TO_MAKEFILE__>", path) for line in lines]

def patch_path_to_proof_root(lines, proof_root, source_root):
    path = os.path.relpath(proof_root, source_root)
    return [line.replace("<__PATH_TO_PROOF_ROOT__>", path) for line in lines]

def patch_path_to_source_file(lines, source_file, source_root):
    path = os.path.relpath(source_file, source_root)
    return [line.replace("<__PATH_TO_SOURCE_FILE__>", path) for line in lines]

def main():
    """Set up CBMC proof."""

    logging.basicConfig(format='%(levelname)s: %(message)s')

    print("What is the function name?  ", end="")
    function = input()

    print("What is the source file that defines the function?  ", end="")
    source_file = os.path.abspath(os.path.expanduser(input()))

    source_root = util.read_source_root()
    proof_root = util.read_proof_root()

    proof_dir = os.path.abspath(function)
    os.mkdir(proof_dir)

    for filename in proof_template_filenames():
        lines = read_proof_template(filename)
        lines = patch_function_name(lines, function)
        lines = patch_path_to_makefile(lines, proof_root, proof_dir)
        lines = patch_path_to_proof_root(lines, proof_root, source_root)
        lines = patch_path_to_source_file(lines, source_file, source_root)
        write_proof_template(lines, filename, proof_dir)

    rename_proof_harness(function, proof_dir)

if __name__ == "__main__":
    main()
