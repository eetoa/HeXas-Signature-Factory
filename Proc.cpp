#include "stdafx.h"

PWTS_PROCESS_INFO Proc::procInfo = NULL;
DWORD             Proc::pCount;			


void Proc::GetProcHandle() {
    g_procHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, g_procId);
}

void Proc::GetProcId()
{
    DWORD procId = 0;
    const wchar_t* procName = Utils::CharToWchar(g_procName.c_str());
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    g_procId = procId;
}

bool Proc::ProcessWalker()
{
    DWORD pLevel = 0; 
    
    // finding process of WTS_CURRENT_SERVER_HANDLE and of WTS_ANY_SESSION
    if (!WTSEnumerateProcessesExW(WTS_CURRENT_SERVER_HANDLE, &pLevel, WTS_ANY_SESSION, (LPWSTR*)&procInfo, &pCount))
    {
        return false;
    }
    else
    {
        for (int i = 1; i <= pCount; i++)
        {
            g_procList.push_back(Utils::LPWSTR_To_String(procInfo->pProcessName));
            procInfo++;
        }

        Utils::SortByDictOrder();

        return true;
    }
}
