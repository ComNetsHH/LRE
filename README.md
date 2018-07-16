# LRE
Standalone implementation of the Limited Relative Error algorithm, including a Python wrapper.

[![DOI](https://zenodo.org/badge/102000011.svg)](https://zenodo.org/badge/latestdoi/102000011)


## Compilation & Verification

Install dependencies (most Debian-based systems)

    sudo apt-get install build-essential libboost-program-options-dev libboost-python-dev python-dev libpython2.7

Compile both, binary and library

    make LRE // For python v2.7
    make LRE3.5 // For python v3.5
    make LRE3.6 // For python v3.6

Running this should show some output

    python LRE_Test.py

Which should look exactly like the output of

    ./LRE --file input.txt --xMin 0 --xMax 500
