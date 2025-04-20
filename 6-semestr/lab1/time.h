#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <stdexcept>
using namespace std;

namespace my_namespace {

class TimeSpan;

class Time {
public:
    Time(int hours = 0, int minutes = 0, int seconds = 0);
    Time operator+(const TimeSpan& span) const;
    Time operator-(const TimeSpan& span) const;
    TimeSpan operator-(const Time& other) const;

    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;

    friend ostream& operator<<(ostream& os, const Time& time);

private:
    int totalSeconds;
};

class TimeSpan {
public:
    TimeSpan(int days = 0, int hours = 0, int minutes = 0, int seconds = 0);
    TimeSpan operator+(const TimeSpan& other) const;
    TimeSpan operator-(const TimeSpan& other) const;

    bool operator==(const TimeSpan& other) const;
    bool operator!=(const TimeSpan& other) const;
    bool operator<(const TimeSpan& other) const;
    bool operator<=(const TimeSpan& other) const;
    bool operator>(const TimeSpan& other) const;
    bool operator>=(const TimeSpan& other) const;

    int getTotalSeconds() const
    {
        return totalSeconds;
    }

    friend ostream& operator<<(ostream& os, const TimeSpan& span);

private:
    int totalSeconds;
};

namespace literals {
TimeSpan operator""_d(unsigned long long days);
TimeSpan operator""_h(unsigned long long hours);
TimeSpan operator""_m(unsigned long long minutes);
TimeSpan operator""_s(unsigned long long seconds);
}

}

#endif