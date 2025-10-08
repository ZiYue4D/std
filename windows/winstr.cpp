#include "../common/std.hpp"
#include <Windows.h>

_STDLIB_BEGIN

ZStr _STDLIB(ConvertToUTF8)(ZStr str) {
    UINT nLen = MultiByteToWideChar(GetACP(), NULL, str->c_str(), -1, NULL, NULL);
    WCHAR* wszBuffer = new WCHAR[nLen + 1];
    nLen = MultiByteToWideChar(GetACP(), NULL, str->c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;

    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    CHAR* szBuffer = new CHAR[nLen + 1];
    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;

    std::string* converted = new std::string(szBuffer);
    delete[] wszBuffer;
    delete[] szBuffer;
    return converted;
}

ZStr _STDLIB(ConvertToANSI)(ZStr str) {
    UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, str->c_str(), -1, NULL, NULL);
    WCHAR* wszBuffer = new WCHAR[nLen + 1];
    nLen = MultiByteToWideChar(CP_UTF8, NULL, str->c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;

    nLen = WideCharToMultiByte(GetACP(), NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    CHAR* szBuffer = new CHAR[nLen + 1];
    nLen = WideCharToMultiByte(GetACP(), NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;

    std::string* converted = new std::string(szBuffer);
    delete[] szBuffer;
    delete[] wszBuffer;
    return converted;
}

_STDLIB_END