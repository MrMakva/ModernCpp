#include "Board.hpp"

int main()
{
    size_t rows, cols, mine_count;
    cin >> rows >> cols >> mine_count;

    vector<pair<int, int>> mines_coords(mine_count);
    for (size_t i = 0; i < mine_count; ++i) {
        cin >> mines_coords[i].first >> mines_coords[i].second;
    }

    Board board(rows, cols, mines_coords);
    string command;

    while (true) {
        board.print(cout);
        cout << "Введите команду (open/flag/exit): ";
        cin >> command;
        if (command == "exit") {
            break;
        }

        int row, col;
        cin >> row >> col;

        if (command == "open") {
            if (!board.open(row, col)) {
                board.print(cout);
                cout << "You lose!\n";
                break;
            }
            if (board.is_won()) {
                board.print(cout);
                cout << "You win!\n";
                break;
            }
        } else if (command == "flag") {
            board.flag(row, col);
        }
    }

    return 0;
}
