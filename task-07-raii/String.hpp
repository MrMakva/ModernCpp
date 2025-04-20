#ifndef STRING_HPP
#define STRING_HPP

#include <cstddef>
#include <cstring>
#include <stdexcept>
using namespace std;

class String {
public:
    String();

    explicit String(const char* str);

    String(const String& other);

    String(String&& other) noexcept;

    String& operator=(const String& other);

    String& operator=(String&& other) noexcept;

    ~String();

    const char* c_str() const;

    char& at(size_t idx);
    const char& at(size_t idx) const;

    size_t size() const;

private:
    char* data;
    size_t length;
};

#endif
