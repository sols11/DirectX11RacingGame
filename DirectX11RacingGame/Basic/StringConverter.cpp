#include "StringConverter.h"

std::wstring StringConverter::StringToWstring(std::string str)
{
    std::wstring wideString(str.begin(), str.end());
    return wideString;
}