#pragma once


class Proc
{
public:
	static PWTS_PROCESS_INFO procInfo;			// using this to hold all the processes
	static DWORD  pCount;								// this will hold all the process count
public:
	static void GetProcId();
	static void GetProcHandle();
	static bool GetProcessList();
	static void GetProcessName(DWORD processID);

	static bool ProcessWalker();

};