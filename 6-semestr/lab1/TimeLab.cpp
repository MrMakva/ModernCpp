#include "time.h"
using namespace std;
namespace my_namespace {

Time::Time(int hours, int minutes, int seconds)
{
    if (hours < 0 || minutes < 0 || seconds < 0) {
        throw invalid_argument("Time values cannot be negative");
    }
    totalSeconds = hours * 3600 + minutes * 60 + seconds;
}

Time Time::operator+(const TimeSpan& span) const
{
    return Time(0, 0, totalSeconds + span.getTotalSeconds());
}

Time Time::operator-(const TimeSpan& span) const
{
    return Time(0, 0, totalSeconds - span.getTotalSeconds());
}

TimeSpan Time::operator-(const Time& other) const
{
    return TimeSpan(0, 0, 0, totalSeconds - other.totalSeconds);
}

bool Time::operator==(const Time& other) const
{
    return totalSeconds == other.totalSeconds;
}

bool Time::operator!=(const Time& other) const
{
    return totalSeconds != other.totalSeconds;
}

bool Time::operator<(const Time& other) const
{
    return totalSeconds < other.totalSeconds;
}

bool Time::operator<=(const Time& other) const
{
    return totalSeconds <= other.totalSeconds;
}

bool Time::operator>(const Time& other) const
{
    return totalSeconds > other.totalSeconds;
}

bool Time::operator>=(const Time& other) const
{
    return totalSeconds >= other.totalSeconds;
}

ostream& operator<<(ostream& os, const Time& time)
{
    int hours = time.totalSeconds / 3600;
    int minutes = (time.totalSeconds % 3600) / 60;
    int seconds = time.totalSeconds % 60;
    os << hours << "h " << minutes << "m " << seconds << "s";
    return os;
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
{
    if (days < 0 || hours < 0 || minutes < 0 || seconds < 0) {
        throw invalid_argument("TimeSpan values cannot be negative");
    }
    totalSeconds = days * 86400 + hours * 3600 + minutes * 60 + seconds;
}

TimeSpan TimeSpan::operator+(const TimeSpan& other) const
{
    return TimeSpan(0, 0, 0, totalSeconds + other.totalSeconds);
}

TimeSpan TimeSpan::operator-(const TimeSpan& other) const
{
    return TimeSpan(0, 0, 0, totalSeconds - other.totalSeconds);
}

bool TimeSpan::operator==(const TimeSpan& other) const
{
    return totalSeconds == other.totalSeconds;
}

bool TimeSpan::operator!=(const TimeSpan& other) const
{
    return totalSeconds != other.totalSeconds;
}

bool TimeSpan::operator<(const TimeSpan& other) const
{
    return totalSeconds < other.totalSeconds;
}

bool TimeSpan::operator<=(const TimeSpan& other) const
{
    return totalSeconds <= other.totalSeconds;
}

bool TimeSpan::operator>(const TimeSpan& other) const
{
    return totalSeconds > other.totalSeconds;
}

bool TimeSpan::operator>=(const TimeSpan& other) const
{
    return totalSeconds >= other.totalSeconds;
}

ostream& operator<<(ostream& os, const TimeSpan& span)
{
    int days = span.totalSeconds / 86400;
    int hours = (span.totalSeconds % 86400) / 3600;
    int minutes = (span.totalSeconds % 3600) / 60;
    int seconds = span.totalSeconds % 60;
    os << days << "d " << hours << "h " << minutes << "m " << seconds << "s";
    return os;
}

namespace literals {
TimeSpan operator""_d(unsigned long long days)
{
    return TimeSpan(static_cast<int>(days), 0, 0, 0);
}

TimeSpan operator""_h(unsigned long long hours)
{
    return TimeSpan(0, static_cast<int>(hours), 0, 0);
}

TimeSpan operator""_m(unsigned long long minutes)
{
    return TimeSpan(0, 0, static_cast<int>(minutes), 0);
}

TimeSpan operator""_s(unsigned long long seconds)
{
    return TimeSpan(0, 0, 0, static_cast<int>(seconds));
}
}

}