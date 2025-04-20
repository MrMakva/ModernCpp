#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
using namespace std;

class Cell {
public:
    bool is_opened_ = false;
    bool has_flag_ = false;
    bool has_mine_ = false;
    size_t mines_count_ = 0;

    char print() const;
};

class Board {
private:
    size_t rows_;
    size_t cols_;
    vector<vector<Cell>> grid_;

    const vector<pair<int, int>> directions
            = {{-1, -1},
               {-1, 0},
               {-1, 1},
               {0, -1},
               {0, 1},
               {1, -1},
               {1, 0},
               {1, 1}};

    bool is_valid(int row, int col) const;
    void count_mines();
    void open_adjacent(int row, int col);

public:
    Board(size_t rows,
          size_t cols,
          const vector<pair<int, int>>& mines_coords);
    bool open(int row, int col);
    void flag(int row, int col);
    void print(ostream& output_stream) const;
    bool is_won() const;
};

#endif
