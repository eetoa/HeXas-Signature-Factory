#pragma once


class Proc
{
public:
	static PWTS_PROCESS_INFO procInfo;
	static DWORD  pCount;			
public:
	static void GetProcHandle();
	static void GetProcId();
	static bool ProcessWalker();
};