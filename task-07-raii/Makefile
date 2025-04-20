CXX = g++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++2a
LDFLAGS = -lgtest -lgtest_main -pthread

SRC = String.cpp String.test.cpp
OBJ = $(SRC:.cpp=.o)

all: string_test

string_test: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f string_test $(OBJ)
