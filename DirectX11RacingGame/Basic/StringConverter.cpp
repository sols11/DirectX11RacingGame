#include "StringConverter.h"

std::wstring StringConverter::StringToWide(std::string str)
{
    std::wstring wideString(str.begin(), str.end());
    return wideString;
}