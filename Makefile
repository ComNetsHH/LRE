CC = g++ -std=c++11

## Python version 2.7
INCLUDE2.7 = -I/usr/include/python
LIBRARIES2.7 = -lpython -lboost_python -lboost_program_options

## Python version 3.5m
INCLUDE3.5 = -I/usr/include/python3.5m
LIBRARIES3.5 = -lpython3.5m -lboost_python3 -lboost_program_options

INCLUDE3.6 = -I/usr/include/python3.6m
LIBRARIES3.6 = -lpython3.6m -lboost_python3 -lboost_program_options

INCLUDE3.8 = -I/usr/include/python3.8
LIBRARIES3.8 = -lpython3.8 -lboost_python3 -lboost_program_options

LRE : *.cc *.h
	$(CC) $(INCLUDE2.7) main/main.cc *.cc -o LRE $(LIBRARIES2.7)
	$(CC) $(INCLUDE2.7) main/main.cc *.cc -o LRE.so $(LIBRARIES2.7) -shared -fPIC

LRE3.5 : *.cc *.h
	$(CC) $(INCLUDE3.5) main/main.cc *.cc -o LRE $(LIBRARIES3.5)
	$(CC) $(INCLUDE3.5) main/main.cc *.cc -o LRE.so $(LIBRARIES3.5) -shared -fPIC

LRE3.6 : *.cc *.h
	$(CC) $(INCLUDE3.6) main/main.cc *.cc -o LRE $(LIBRARIES3.6)
	$(CC) $(INCLUDE3.6) main/main.cc *.cc -o LRE.so $(LIBRARIES3.6) -shared -fPIC

LRE3.8 : *.cc *.h
	$(CC) $(INCLUDE3.8) main/main.cc *.cc -o LRE $(LIBRARIES3.8)
	$(CC) $(INCLUDE3.8) main/main.cc *.cc -o LRE.so $(LIBRARIES3.8) -shared -fPIC
