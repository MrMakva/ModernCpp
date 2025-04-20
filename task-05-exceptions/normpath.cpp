#include "normpath.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

string normpath(const string& path)
{
    if (path.empty() || path[0] != '/') {
        throw invalid_argument("invalid argument, not an absolute path");
    }

    if (path.find(' ') != string::npos) {
        throw domain_error("domain_error, spaces are not allowed");
    }

    vector<string> parts;
    string current_part;

    for (const char c : path) {
        if (c == '/') {
            if (!current_part.empty()) {
                parts.push_back(current_part);
                current_part.clear();
            }
        } else {
            current_part += c;
        }
    }

    if (!current_part.empty()) {
        parts.push_back(current_part);
    }

    vector<string> result;

    for (const auto& part : parts) {
        if (part == "." || part.empty()) {
            continue;
        } else if (part == "..") {
            if (!result.empty()) {
                result.pop_back();
            } else {
                throw runtime_error(
                        "runtime_error, cannot normalize path");
            }
        } else {
            result.push_back(part);
        }
    }

    string normalized_path = "/" + string();
    for (const auto& part : result) {
        normalized_path += part + "/";
    }

    if (normalized_path.length() > 1) {
        normalized_path.pop_back();
    }

    return normalized_path;
}
