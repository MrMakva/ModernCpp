#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

string soundex(const string& name)
{
    if (name.empty() || !all_of(name.begin(), name.end(), [](char c) {
            return isalpha(c) || isspace(c);
        })) {
        return "0000";
    }

    const char firstLetter = toupper(name[0]);

    string filtered;
    filtered.reserve(name.size() - 1);

    copy_if(
            name.begin() + 1,
            name.end(),
            back_inserter(filtered),
            [](char c) {
                return c != 'h' && c != 'H' && c != 'w' && c != 'W';
            });

    vector<char> soundexMap(26, '0');

    soundexMap['b' - 'a'] = '1';
    soundexMap['f' - 'a'] = '1';
    soundexMap['p' - 'a'] = '1';
    soundexMap['v' - 'a'] = '1';
    soundexMap['c' - 'a'] = '2';
    soundexMap['g' - 'a'] = '2';
    soundexMap['j' - 'a'] = '2';
    soundexMap['k' - 'a'] = '2';
    soundexMap['q' - 'a'] = '2';
    soundexMap['s' - 'a'] = '2';
    soundexMap['x' - 'a'] = '2';
    soundexMap['z' - 'a'] = '2';
    soundexMap['d' - 'a'] = '3';
    soundexMap['t' - 'a'] = '3';
    soundexMap['l' - 'a'] = '4';
    soundexMap['m' - 'a'] = '5';
    soundexMap['n' - 'a'] = '5';
    soundexMap['r' - 'a'] = '6';

    string soundexCode(1, firstLetter);
    char prevDigit = '0';

    for_each(
            filtered.begin(),
            filtered.end(),
            [&soundexCode, &prevDigit, &soundexMap](char c) {
                const char lowerChar = tolower(c);
                const char currentDigit = soundexMap[lowerChar - 'a'];
                if (currentDigit != prevDigit) {
                    soundexCode += currentDigit;
                }
                prevDigit = currentDigit;
            });

    soundexCode.erase(
            remove_if(
                    soundexCode.begin() + 1,
                    soundexCode.end(),
                    [](char c) {
                        return c == '0' || c == 'a' || c == 'e' || c == 'i'
                                || c == 'o' || c == 'u' || c == 'y';
                    }),
            soundexCode.end());

    soundexCode = soundexCode.substr(0, 4);
    soundexCode.append(4 - soundexCode.size(), '0');

    return soundexCode;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: soundex <name>\n";
        return 1;
    }

    const string name = argv[1];
    cout << soundex(name) << '\n';

    return 0;
}