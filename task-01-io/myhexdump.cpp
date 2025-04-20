#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

void printHexDump(istream& inputStream)
{
    unsigned char buffer[16];
    streamsize offset = 0;

    while (inputStream.read(reinterpret_cast<char*>(buffer), sizeof(buffer))
           || inputStream.gcount() > 0) {
        const streamsize bytesRead = inputStream.gcount();

        cout << setw(8) << setfill('0') << hex << offset
                  << "  ";
        cout << dec;

        for (streamsize i = 0; i < 16; i++) {
            if (i < bytesRead) {
                cout << setw(2) << setfill('0') << hex
                          << static_cast<int>(buffer[i]);
            } else {
                cout << "  ";
            }
            if (i == 7) {
                cout << "  ";
            }
            if (i < 15) {
                cout << " ";
            }
        }

        cout << "  |";

        for (streamsize i = 0; i < 16; i++) {
            if (i < bytesRead) {
                const unsigned char c = buffer[i];
                if (isprint(c)) {
                    cout << c;
                } else {
                    cout << '.';
                }
            } else {
                cout << ' ';
            }
        }

        cout << "|" << '\n';

        offset += 16;
    }
}

int main(int argc, char* argv[])
{
    istream* inputStream = &cin;
    ifstream inputFile;

    if (argc >= 2) {
        inputFile.open(argv[1]);
        if (!inputFile) {
            cerr << "Ошибка при открытии файла: " << argv[1] << '\n';
            return 1;
        }
        inputStream = &inputFile;
    } else if (argc > 2) {
        cerr << "Использование: " << argv[0] << " [имя_файла]" << '\n';
        return 1;
    }

    printHexDump(*inputStream);

    if (inputFile.is_open()) {
        inputFile.close();
    }

    return 0;
}
