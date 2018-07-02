CC = g++ -std=c++11

## Python version 2.7
INCLUDE2.7 = -I/usr/include/python
LIBRARIES2.7 = -lpython -lboost_python -lboost_program_options

## Python version 3.5m
INCLUDE3.5 = -I/usr/include/python3.5m
LIBRARIES3.5 = -lpython3.5m -lboost_python3 -lboost_program_options

INCLUDE3.6 = -I/usr/include/python3.6m
LIBRARIES3.6 = -lpython3.6m -lboost_python3 -lboost_program_options

LRE : *.cpp *.hpp
	$(CC) $(INCLUDE2.7) *.cpp -o LRE $(LIBRARIES2.7)
	$(CC) $(INCLUDE2.7) *.cpp -o LRE.so $(LIBRARIES2.7) -shared -fPIC

LRE3.5 : *.cpp *.hpp
	$(CC) $(INCLUDE3.5) *.cpp -o LRE $(LIBRARIES3.5)
	$(CC) $(INCLUDE3.5) *.cpp -o LRE.so $(LIBRARIES3.5) -shared -fPIC

LRE3.6 : *.cpp *.hpp
	$(CC) $(INCLUDE3.6) *.cpp -o LRE $(LIBRARIES3.6)
	$(CC) $(INCLUDE3.6) *.cpp -o LRE.so $(LIBRARIES3.6) -shared -fPIC
