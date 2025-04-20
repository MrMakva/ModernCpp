#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
using namespace std;

using std::accumulate;
using std::array;
using std::cerr;
using std::cin;
using std::count;
using std::cout;
using std::includes;
using std::map;
using std::sort;
using std::vector;

int calculateOnes(const array<int, 5>& dice)
{
    return count(dice.begin(), dice.end(), 1);
}

int calculateTwos(const array<int, 5>& dice)
{
    return count(dice.begin(), dice.end(), 2) * 2;
}

int calculateThrees(const array<int, 5>& dice)
{
    return count(dice.begin(), dice.end(), 3) * 3;
}

int calculateFours(const array<int, 5>& dice)
{
    return count(dice.begin(), dice.end(), 4) * 4;
}

int calculateFives(const array<int, 5>& dice)
{
    return count(dice.begin(), dice.end(), 5) * 5;
}

int calculateSixes(const array<int, 5>& dice)
{
    return count(dice.begin(), dice.end(), 6) * 6;
}

int calculateThreeOfAKind(const array<int, 5>& dice)
{
    vector<int> counts(7, 0);
    for (const int die : dice) {
        counts[die]++;
    }
    for (int i = 1; i <= 6; ++i) {
        if (counts[i] >= 3) {
            return accumulate(dice.begin(), dice.end(), 0);
        }
    }
    return 0;
}

int calculateFourOfAKind(const array<int, 5>& dice)
{
    vector<int> counts(7, 0);
    for (const int die : dice) {
        counts[die]++;
    }
    for (int i = 1; i <= 6; ++i) {
        if (counts[i] >= 4) {
            return accumulate(dice.begin(), dice.end(), 0);
        }
    }
    return 0;
}

int calculateFullHouse(const array<int, 5>& dice)
{
    vector<int> counts(7, 0);
    for (const int die : dice) {
        counts[die]++;
    }
    bool hasPair = false;
    bool hasThree = false;
    for (int i = 1; i <= 6; ++i) {
        if (counts[i] == 2) {
            hasPair = true;
        } else if (counts[i] == 3) {
            hasThree = true;
        }
    }
    return (hasPair && hasThree) ? 25 : 0;
}

int calculateSmallStraight(const array<int, 5>& dice)
{
    vector<int> uniqueDice(dice.begin(), dice.end());
    sort(uniqueDice.begin(), uniqueDice.end());
    uniqueDice.erase(
            unique(uniqueDice.begin(), uniqueDice.end()), uniqueDice.end());

    const vector<vector<int>> smallStraights
            = {{1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}};

    for (const auto& straight : smallStraights) {
        if (includes(
                    uniqueDice.begin(),
                    uniqueDice.end(),
                    straight.begin(),
                    straight.end())) {
            return 30;
        }
    }
    return 0;
}

int calculateLargeStraight(const array<int, 5>& dice)
{
    vector<int> uniqueDice(dice.begin(), dice.end());
    sort(uniqueDice.begin(), uniqueDice.end());
    uniqueDice.erase(
            unique(uniqueDice.begin(), uniqueDice.end()), uniqueDice.end());

    if (uniqueDice == vector<int>{1, 2, 3, 4, 5}
        || uniqueDice == vector<int>{2, 3, 4, 5, 6}) {
        return 40;
    }
    return 0;
}

int calculateYahtzee(const array<int, 5>& dice)
{
    vector<int> counts(7, 0);
    for (const int die : dice) {
        counts[die]++;
    }
    for (int i = 1; i <= 6; ++i) {
        if (counts[i] == 5) {
            return 50;
        }
    }
    return 0;
}

int calculateChance(const array<int, 5>& dice)
{
    return accumulate(dice.begin(), dice.end(), 0);
}

int main(int argc, char* argv[])
{
    if (argc != 6) {
        cerr << "Error: You must enter exactly five dice values (1-6).\n";
        return 1;
    }

    array<int, 5> dice;
    for (int i = 0; i < 5; ++i) {
        dice[i] = atoi(argv[i + 1]);
        if (dice[i] < 1 || dice[i] > 6) {
            cerr << "Error: invalid dice value " << dice[i] << '\n';
            return 1;
        }
    }

    cout << "Dice values: ";
    for (const auto& die : dice) {
        cout << die << " ";
    }
    cout << '\n';

    cout << "Ones:             " << calculateOnes(dice) << '\n';
    cout << "Twos:             " << calculateTwos(dice) << '\n';
    cout << "Threes:           " << calculateThrees(dice) << '\n';
    cout << "Fours:            " << calculateFours(dice) << '\n';
    cout << "Fives:            " << calculateFives(dice) << '\n';
    cout << "Sixes:            " << calculateSixes(dice) << '\n';
    cout << "Three Of A Kind:  " << calculateThreeOfAKind(dice) << '\n';
    cout << "Four Of A Kind:   " << calculateFourOfAKind(dice) << '\n';
    cout << "Full House:       " << calculateFullHouse(dice) << '\n';
    cout << "Small Straight:    " << calculateSmallStraight(dice) << '\n';
    cout << "Large Straight:    " << calculateLargeStraight(dice) << '\n';
    cout << "Yahtzee:         " << calculateYahtzee(dice) << '\n';
    cout << "Chance:           " << calculateChance(dice) << '\n';

    return 0;
}