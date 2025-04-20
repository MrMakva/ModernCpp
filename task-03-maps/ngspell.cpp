#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::max;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

vector<string> getTrigrams(const string& word)
{
    vector<string> trigrams;
    for (size_t i = 0; i <= word.size() - 3; ++i) {
        trigrams.push_back(word.substr(i, 3));
    }
    return trigrams;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <dict_file_path>" << '\n';
        return 1;
    }

    const string dictFilePath = argv[1];
    ifstream dictFile(dictFilePath);

    if (!dictFile) {
        cerr << "Could not open dictionary file: " << dictFilePath << '\n';
        return 1;
    }

    unordered_map<string, set<string>> reverseIndex;

    string word;
    while (getline(dictFile, word)) {
        if (word.size() < 3)
            continue;

        const vector<string> trigrams = getTrigrams(word);
        for (const string& trigram : trigrams) {
            reverseIndex[trigram].insert(word);
        }
    }

    dictFile.close();

    cout << "Введите слова для поиска (введите ':q' для выхода):" << '\n';

    while (true) {
        cout << "> ";
        string query;
        getline(cin, query);

        if (query == ":q")
            break;

        const vector<string> queryTrigrams = getTrigrams(query);
        unordered_map<string, int> candidateCount;

        for (const string& trigram : queryTrigrams) {
            if (reverseIndex.find(trigram) != reverseIndex.end()) {
                for (const string& candidate : reverseIndex[trigram]) {
                    candidateCount[candidate]++;
                }
            }
        }

        int maxCount = 0;
        for (const auto& pair : candidateCount) {
            maxCount = max(maxCount, pair.second);
        }

        vector<string> results;
        for (const auto& pair : candidateCount) {
            if (pair.second == maxCount) {
                results.push_back(pair.first);
            }
        }

        sort(results.begin(), results.end());

        cout << results.size() << '\n';
        for (const string& result : results) {
            cout << result << '\n';
        }
    }

    return 0;
}
