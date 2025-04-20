#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    int kolvo = 10;
    bool lines = true;
    istream* inputStream = &cin;
    ifstream inputFile;

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-n" && i + 1 < argc) {
            lines = true;
            kolvo = stoi(argv[++i]);
        } else if (string(argv[i]) == "-c" && i + 1 < argc) {
            lines = false;
            kolvo = stoi(argv[++i]);
        } else if (inputFile.is_open() == false) {
            inputFile.open(argv[i]);
            if (!inputFile) {
                cerr << "Ошибка при открытии файла: " << argv[i] << '\n';
                return 1;
            }
            inputStream = &inputFile;
        }
    }

    if (lines) {
        string line;
        for (int count = 0; count < kolvo && getline(*inputStream, line);
             ++count) {
            cout << line << '\n';
        }
    } else {
        char byte;
        for (int count = 0; count < kolvo && inputStream->get(byte); ++count) {
            cout << byte;
        }
    }

    if (inputFile.is_open()) {
        inputFile.close();
    }

    return 0;
}
