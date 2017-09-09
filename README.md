# LRE
Standalone implementation of the Limited Relative Error algorithm including a Python wrapper

## Compilation & Verification

Install dependencies (most Debian-based systems)

    sudo apt-get install build-essential libboost-program-options-dev libboost-python-dev python-dev libpython2.7

Compile both, binary and library

    make 		// For python v2.7
    make LRE3.5 // For python v3.5

Running this should show some output

    python LRE_Test.py

Which should look exactly like the output of

    ./LRE --file input.txt --xMin 0 --xMax 1