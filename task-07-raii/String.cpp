#include "String.hpp"

String::String() : data(nullptr), length(0)
{
}

String::String(const char* str)
{
    if (str) {
        length = strlen(str);
        data = new char[length + 1];
        strncpy(data, str, length + 1);
    } else {
        data = nullptr;
        length = 0;
    }
}

String::String(const String& other) : length(other.length)
{
    if (other.data) {
        data = new char[length + 1];
        strncpy(data, other.data, length + 1);
    } else {
        data = nullptr;
    }
}

String::String(String&& other) noexcept : data(other.data), length(other.length)
{
    other.data = nullptr;
    other.length = 0;
}

String& String::operator=(const String& other)
{
    if (this != &other) {
        delete[] data;
        length = other.length;
        data = new char[length + 1];
        strncpy(data, other.data, length + 1);
    }
    return *this;
}

String& String::operator=(String&& other) noexcept
{
    if (this != &other) {
        delete[] data;
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
    }
    return *this;
}

String::~String()
{
    delete[] data;
}

const char* String::c_str() const
{
    return data ? data : "";
}

char& String::at(size_t idx)
{
    if (idx >= length)
        throw out_of_range("Index out of range");
    return data[idx];
}

const char& String::at(size_t idx) const
{
    if (idx >= length)
        throw out_of_range("Index out of range");
    return data[idx];
}

size_t String::size() const
{
    return length;
}
