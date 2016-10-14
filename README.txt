Install needed programs:
  sudo apt-get install libboost-program-options-dev libboost-python-dev python-dev libpython2.7

Compile both binary and library:
  make // For python v2.7
  make LRE3.5 // For python v3.5m

Running this should output some stuff:
  python LRE_Test.py

Which should look exactly like the output of this:
  ./LRE --file input.txt --xMin 0 --xMax 1
