#include "stdafx.h"


const wchar_t* Utils::CharToWchar(const char* ch)
{
    const size_t len = strlen(ch) + 1;
    wchar_t* wch = new wchar_t[len];
    mbstowcs(wch, ch, len);
    return wch;
}


bool cmp(std::string s1, std::string s2)
{
    return strcmp(s1.c_str(), s2.c_str()) < 0;
}

void Utils::SortByDictOrder()
{
    std::sort(g_procList.begin(), g_procList.end(), cmp);
}