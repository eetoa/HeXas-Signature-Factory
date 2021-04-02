#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <psapi.h>
#include <tchar.h>
#include <string>
#include <algorithm>
#include "color.hpp"
#include <conio.h>

HANDLE g_hConsole;
const char* g_procName = "CoDWaW.exe";
uintptr_t g_address;
unsigned int g_key;
uintptr_t g_size;
HANDLE	g_procHandle;
DWORD	g_procId;

std::vector<std::string> g_procList;

enum Option
{
	GetProcList = 1,
	ReGenerateSig = 2,
	GenerateSigByProcessName = 3,
	GenerateSigByPrimarykey = 4,
	ErrorChoice = -1,
};

template <typename T>
T ReadEx(uintptr_t address)
{
	T VALUE;

	unsigned long preProtection;
	VirtualProtectEx(g_procHandle, reinterpret_cast<void*>(address), sizeof(address), PAGE_EXECUTE_READWRITE, &preProtection);

	ReadProcessMemory(g_procHandle, (LPCVOID)(address), &VALUE, sizeof(VALUE), 0);

	VirtualProtectEx(g_procHandle, reinterpret_cast<void*>(address), sizeof(address), preProtection, &preProtection);

	return VALUE;
}