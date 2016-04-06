INCLUDE = -I/usr/include/python3.5m -lboost_python3 -lpython3.5m -lboost_program_options
TARGET = LRE

all: $(TARGET).so $(TARGET)
# Shared library.
$(TARGET).so: LRE.cpp
	g++ -std=c++11 $(TARGET).cpp $(INCLUDE) -o $(TARGET).so -shared -fPIC
# Native program.
$(TARGET): LRE.cpp
	g++ -std=c++11 $(INCLUDE) $(TARGET).cpp -o $(TARGET)
