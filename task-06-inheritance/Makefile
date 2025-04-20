CXX = g++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++2a
TARGET = calc

all: $(TARGET)

$(TARGET): calc.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) calc.cpp

clean:
	rm -f $(TARGET)
