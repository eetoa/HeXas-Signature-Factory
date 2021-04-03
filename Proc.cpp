#include "stdafx.h"

PWTS_PROCESS_INFO Proc::procInfo = NULL;
DWORD             Proc::pCount;			

void Proc::GetProcessName(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    std::string procName;
#ifndef UNICODE
    str = szProcessName;
#else
    std::wstring wStr = szProcessName;
    procName = std::string(wStr.begin(), wStr.end());
#endif

    if (procName != "<unknown>")
    {
        //std::cout << procName.c_str() << std::endl;
        g_procList.push_back(procName);
    }

    // Release the handle to the process.
    CloseHandle(hProcess);
}

bool Proc::GetProcessList()
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return false;
    }

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            GetProcessName(aProcesses[i]);
        }
    }

    return 0;
}

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
        using namespace std;
        cout << endl;
        cout << setw(8) << right << "Key" << "        " << setw(30) << left << "Process Name" << endl;

        for (int i = 1; i <= pCount; i++)
        {
            cout << setw(8) << right << to_string(i) << "        " << setw(30) << left << Utils::LPWSTR_To_String(procInfo->pProcessName) << endl;
            procInfo++;
        }
        return true;
    }
}
