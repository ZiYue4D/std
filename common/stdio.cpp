#include "std.hpp"

#include <iostream>

_STDLIB_BEGIN
_ALWAYS_INLINE
void _STDLIB(Print)(ZStr str) {
    puts(str->c_str());
}

_ALWAYS_INLINE
void _STDLIB(Write)(ZStr str) {
    std::cout << str;
}

ZStr _STDLIB(Input)(ZStr prompt) {
    std::string line;
    std::cout << *prompt;
    std::cin >> line;
    return new std::string(line);
}

ZStr _STDLIB(GetCommandLineArg)(char** argv, int index) {
    return new std::string(reinterpret_cast<const char*>(argv[index]));
}

_STDLIB_END
