#include "std.hpp"

#define XXH_INLINE_ALL
#include "xxhash.h"

_STDLIB_BEGIN

__int128 _STDLIB(string_hash_128__)(ZStr str) {
    const auto [low64, high64] = XXH3_128bits(str->data(), str->size());
    return static_cast<__int128>(high64) << 64 | low64;
}

ZStr _STDLIB(create_string__)(const char* raw) {
    return new std::string(raw);
}

void _STDLIB(release_string__)(ZStr str) {
    // danger! do not imitate
    delete const_cast<std::string*>(str);
}

ZStr _STDLIB(int_to_string__)(int raw) {
    return new std::string(std::to_string(raw));
}

ZStr _STDLIB(float_to_string__)(float raw) {
    return new std::string(std::to_string(raw));
}

_STDLIB_END