#include "normpath.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <absolute path>" << '\n';
        return 1;
    }

    try {
        const string result = normpath(argv[1]);
        cout << result << '\n';
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (const domain_error& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (...) {
        cerr << "Error: unknown exception occurred" << '\n';
        return 1;
    }

    return 0;
}
