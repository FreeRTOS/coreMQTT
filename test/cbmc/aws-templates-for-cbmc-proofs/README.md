# AWS Templates for CBMC Proofs

This repository includes templates and scripts intended to make it
easy to get started writing CBMC proofs for your code.  The first
script installs proof infrastructure in the form of a few directories
containing code, templates, and Makefiles that facilitate writing CBMC proofs
in general.  The second script installs, for a particular proof, a directory
containing skeletons of all the files needed to start writing that proof.

## Proof infrastructure set up

The script scripts/setup.py will set up the infrastructure for CBMC proofs.

* Choose the path to the source root (eg, /usr/project)
* Choose the path to a directory under the source root that should hold
  the infrastructure (eg, /usr/project/cbmc)
* Submodule the AWS-templates-for-CBMC repository into this directory (eg,
  /usr/project/cbmc/aws-templates-for-cbmc)
  ```
  cd /usr/project/cbmc
  git submodule add https://github.com/awslabs/aws-templates-for-cbmc-proofs.git aws-templates-for-cbmc-proofs
  ```
* Use the script `aws-templates-for-cbmc-proofs/scripts/setup.py` to
  setup the standard directory structure for CBMC proof.
  ```
  cd /usr/project/cbmc
  python3 aws-templates-for-cbmc-proofs/scripts/setup.py
  ```
  The script will ask for the path to the source root `/usr/project`.
  The script will install the directories
  * include: contains include files for the proofs
  * sources: contains source code for the proofs
  * stubs: contains stubs for the proofs
  * proofs: contains the proofs themselves (the proof root).

## Proof set up

The script scripts/setup-proof.py will set up a directory for a CBMC proof.

* Change to a directory under the proof root (/usr/project/cbmc/proofs)
* Run the script `aws-templates-for-cbmc/scripts/setup-proof.py` and give
  * The name of the function under test.
  * The path to the source file defining the function under test.
  * The path to the source root

The script will create a directory named for the function, and will
install files needed to run the proof.

You can cut and paste into the files, and type `make` to run and debug
the proof.  For more details, see [the instructions in the training
materials repository](github.com/awslabs/aws-training-materials-for-cbmc/SETUP.md).

## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

This project is licensed under the Apache-2.0 License.
