# Pre-requisites to running the spell check scripts

1. In your GNU environment, install the *spell* and *getopt* programs. Use the following commands in Debian distributions, to install the packages (*getopt* is part of the `util-linux` package):
   ```shell
   apt-get install spell
   apt-get install util-linux
   ```

1. Add the folder containing the **tools/spell/ablexicon**, **tools/spell/extract-comments**, and **tools/spell/find-unknown-comment-words** scripts to your system's PATH.
   ```shell
   export PATH=<REPO_ROOT>/tools/spell:$PATH
   ```

# How to create a lexicon.txt for a new directory.

1. Ensure there does not exist a file called "lexicon.txt" in the directory. Run the following command to create a lexicon.txt for the directory:
   ```shell
   find-unknown-comment-words -d <PATH>/<TO>/<DIRECTORY> > <PATH>/<TO>/<DIRECTORY>/lexicon.txt
   ```

1. Check the contents of *<PATH>/<TO>/<DIRECTORY>* for any misspelled words. Fix them in your library's source code and delete them from the lexicon.txt.

# How to run for changes to an existing directory containing lexicon.txt file.

1. If there exists a lexicon.txt in a directory, run the following command:
   ```shell
   find-unknown-comment-words -d <PATH>/<TO>/<DIRECTORY>
   ```

1. Add any non-dictionary correctly spelled words to *<PATH>/<TO>/<DIRECTORY>/lexicon.txt*. Fix any misspelled words in your code comment change.