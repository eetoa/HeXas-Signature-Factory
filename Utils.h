#pragma once

class Utils
{
public:
	static std::string ToHexString(unsigned int n);
	static const wchar_t* CharToWchar(const char* ch);
	static void SortByDictOrder();
	static std::string LPWSTR_To_String(LPWSTR lpwstr);
};