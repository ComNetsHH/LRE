sudo apt-get install libboost-program-options-dev libboost-python-dev python-dev libpython2.7

g++ LRE.cpp -I/usr/include/python2.7 -lpython2.7 -lboost_python -lboost_program_options -o LRE.so -shared -std=c++11 -fPIC
g++ LRE.cpp -I/usr/include/python2.7 -lpython2.7 -lboost_python -lboost_program_options -o LRE -std=c++11

python LRE_Test.py
