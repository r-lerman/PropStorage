#pragma once
#include <regex>

inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isspace(c); }));
}

inline std::string trim(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isspace(c); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), s.end());
    return s;
}

inline bool isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c) && c != '+' && c != '-' && c != '.'; }) == s.end();
}

inline void SplitText(const std::string& text, std::string& first, std::string& second, const std::string& delimiter)
{
    std::string::size_type idx = text.find_first_of(delimiter);
    if (idx == std::string::npos)
    {
        first = trim(text);
        second.clear();
    }
    else
    {
        first = trim(text.substr(0, idx));
        second = text.substr(idx + delimiter.length());
    }
}

