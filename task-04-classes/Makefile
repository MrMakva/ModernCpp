CXX = g++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++2a

all: mines

mines: Board.o mines.o
	$(CXX) -o mines Board.o mines.o

Board.o: Board.cpp Board.hpp
	$(CXX) $(CXXFLAGS) -c Board.cpp

mines.o: mines.cpp Board.hpp
	$(CXX) $(CXXFLAGS) -c mines.cpp

clean:
	rm -f *.o mines
