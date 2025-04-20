#include "Board.hpp"

char Cell::print() const
{
    if (has_flag_)
        return 'F';
    if (!is_opened_)
        return '#';
    if (has_mine_)
        return '*';
    return mines_count_ == 0 ? '.' : '0' + mines_count_;
}

Board::Board(
        size_t rows,
        size_t cols,
        const vector<pair<int, int>>& mines_coords)
    : rows_(rows), cols_(cols), grid_(rows, vector<Cell>(cols))
{
    for (const auto& coord : mines_coords) {
        grid_[coord.first][coord.second].has_mine_ = true;
    }
    count_mines();
}

bool Board::is_valid(int row, int col) const
{
    return row >= 0 && row < static_cast<int>(rows_) && col >= 0
            && col < static_cast<int>(cols_);
}

void Board::count_mines()
{
    for (size_t r = 0; r < rows_; ++r) {
        for (size_t c = 0; c < cols_; ++c) {
            if (grid_[r][c].has_mine_)
                continue;

            for (const auto& dir : directions) {
                const int new_row = r + dir.first;
                const int new_col = c + dir.second;
                if (is_valid(new_row, new_col)
                    && grid_[new_row][new_col].has_mine_) {
                    grid_[r][c].mines_count_++;
                }
            }
        }
    }
}

void Board::open_adjacent(int row, int col)
{
    if (!is_valid(row, col) || grid_[row][col].is_opened_
        || grid_[row][col].has_flag_)
        return;

    grid_[row][col].is_opened_ = true;

    if (grid_[row][col].mines_count_ == 0) {
        for (const auto& dir : directions) {
            open_adjacent(row + dir.first, col + dir.second);
        }
    }
}

bool Board::open(int row, int col)
{
    if (!is_valid(row, col) || grid_[row][col].is_opened_
        || grid_[row][col].has_flag_) {
        return true;
    }

    if (grid_[row][col].has_mine_) {
        grid_[row][col].is_opened_ = true;
        return false;
    }

    open_adjacent(row, col);
    return true;
}

void Board::flag(int row, int col)
{
    if (is_valid(row, col)) {
        grid_[row][col].has_flag_ = !grid_[row][col].has_flag_;
    }
}

void Board::print(ostream& output_stream) const
{
    for (const auto& row : grid_) {
        for (const auto& cell : row) {
            output_stream << cell.print();
        }
        output_stream << '\n';
    }
}

bool Board::is_won() const
{
    for (const auto& row : grid_) {
        for (const auto& cell : row) {
            if (!cell.is_opened_ && !cell.has_mine_) {
                return false;
            }
        }
    }
    return true;
}
