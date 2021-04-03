#include "stdafx.h"


std::string Utils::ToHexString(unsigned int n)
{
	if (!n) return "00";				// special situration
	std::string res = "";
	unsigned int stk[100] = {};
	int tt = 0;
	for (; n; n /= 16) {
		stk[++tt] = n % 16;				// push
	}
	for (; tt; tt--) {
		switch (stk[tt]) {				// pop
		case 10:res += "A"; break;
		case 11:res += "B"; break;
		case 12:res += "C"; break;
		case 13:res += "D"; break;
		case 14:res += "E"; break;
		case 15:res += "F"; break;
		default:res += std::to_string(stk[tt]); break;
		}
	}
	return res;
}

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