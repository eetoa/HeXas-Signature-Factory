#pragma once

class Proc
{
public:
	static void GetProcId();
	static void GetProcHandle();
	static bool GetProcessList();
	static void GetProcessName(DWORD processID);

};