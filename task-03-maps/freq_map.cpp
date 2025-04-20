#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error opening file: " << argv[1] << '\n';
        return 1;
    }

    vector<string> words;
    string word;
    while (file >> word) {
        words.push_back(word);
    }

    map<string, int> word_count;

    const auto start_count = chrono::steady_clock::now();

    for (const auto& w : words) {
        word_count[w]++;
    }

    const auto finish_count = chrono::steady_clock::now();
    const auto duration_count
            = chrono::duration_cast<chrono::microseconds>(
                    finish_count - start_count);

    vector<pair<string, int>> sorted_words(
            word_count.begin(), word_count.end());

    const auto start_sort = chrono::steady_clock::now();

    sort(
            sorted_words.begin(),
            sorted_words.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second
                        || (a.second == b.second && a.first < b.first);
            });

    const auto finish_sort = chrono::steady_clock::now();
    const auto duration_sort
            = chrono::duration_cast<chrono::microseconds>(
                    finish_sort - start_sort);

    for (size_t i = 0;
         i < min(sorted_words.size(), static_cast<size_t>(10));
         ++i) {
        cout << sorted_words[i].second << ' ' << sorted_words[i].first
                  << '\n';
    }

    cout << "build: " << duration_count.count() << '\n';
    cout << "sort: " << duration_sort.count() << '\n';

    return 0;
}
