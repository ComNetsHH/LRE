CC = g++ -std=c++11

## Python version 2.7
INCLUDE2.7 = -I/usr/include/python2.7
LIBRARIES2.7 = -lpython2.7 -lboost_python -lboost_program_options

## Python version 3.5m
INCLUDE = -I/usr/include/python3.5m
LIBRARIES = -lpython3.5m -lboost_python3 -lboost_program_options

.PHONY : LRE

LRE : *.cpp *.hpp
	$(CC) $(INCLUDE2.7) LRE.cpp -o LRE $(LIBRARIES2.7)
	$(CC) $(INCLUDE2.7) LRE.cpp -o LRE.so $(LIBRARIES2.7) -shared -fPIC

LRE3.5 : *cpp *.hpp
	$(CC) $(INCLUDE) LRE.cpp -o LRE $(LIBRARIES)
	$(CC) $(INCLUDE) LRE.cpp -o LRE.so $(LIBRARIES) -shared -fPIC
