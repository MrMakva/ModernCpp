CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = normpath

all: $(TARGET)

$(TARGET): main.o normpath.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o normpath.o

main.o: main.cpp normpath.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

normpath.o: normpath.cpp normpath.hpp
	$(CXX) $(CXXFLAGS) -c normpath.cpp

clean:
	rm -f *.o $(TARGET)
