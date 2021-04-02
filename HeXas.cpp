#include "stdafx.h"
#include "color.hpp"

HANDLE g_hConsole;
std::string g_procName;
uintptr_t g_address;
unsigned int g_key;
uintptr_t g_size;
HANDLE	g_procHandle;
DWORD	g_procId;
std::vector<std::string> argv;
std::vector<std::string> g_procList;

namespace Info {

    using namespace std;
    inline void printHelp()
    {
        cout << "Help Information" << endl << endl;
    }
}

const wchar_t* CharToWchar(const char* ch)
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

void SortByDictOrder()
{
    std::sort(g_procList.begin(), g_procList.end(), cmp);
}

void GetProcessName(DWORD processID)
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

bool GetProcessList()
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

void GetProcHandle() {
    g_procHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, g_procId);
}

void GetProcId()
{
    DWORD procId = 0;
    const wchar_t* procName = CharToWchar(g_procName.c_str());
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

bool GenerateSig()
{
    GetProcId();
    GetProcHandle();
    std::cout << g_procId << " - " << g_procHandle << std::endl;

    std::vector<byte> res;
    for (unsigned int i = 0; i < g_size; i++) {
        byte cur;
        res.push_back(ReadEx<byte>(g_address + i));
    }

    for (int i = 0; i < res.size(); i++) {
        std::cout << std::hex << dye::on_light_blue((unsigned int)res[i]) << dye::on_light_blue(" ");
    }
    std::cout << std::endl << std::endl;
}

Option Parse(std::vector<std::string> cmd)
{
    int len = cmd.size();
    if (len != 2 && len != 4 && len != 7) return ErrorChoice;
    //std::cout << argc << std::endl;

    if (len == 2 && !strcmp(cmd[1].c_str(), "-ls"))   // hexas -ls
    {
        return GetProcList;
    }

    if (len == 4 && !strcmp(cmd[1].c_str(), "-r") && !strcmp(cmd[2].c_str(), "-a"))  // hexas -r -a 
    {
        long add;
        char* ptr;
        add = strtol(cmd[3].c_str(), &ptr, 16);
        g_address = (uintptr_t)add;
        return ReGenerateSig;
    }
    /*
    for (int i = 0; i < argc; i++) {
        std::cout << i << " - " << argv[i] << std::endl;
    }
    */
    if (len == 7 && !strcmp(cmd[1].c_str(), "-p") && !strcmp(cmd[3].c_str(), "-a") && !strcmp(cmd[5].c_str(), "-s")) {   // hexas -p [name or key] -a [address] -s [size]
        //std::cout << argc << std::endl;

        char* ptr;

        // parse address
        long add;
        add = strtol(cmd[4].c_str(), &ptr, 16);
        g_address = (uintptr_t)add;
        std::cout << std::hex << g_address << std::endl;

        // parse size
        add = strtol(cmd[6].c_str(), &ptr, 16);
        g_size = (uintptr_t)add;
        std::cout << std::hex << g_size << std::endl;

        // determin name or key
        std::string s;
        s = cmd[2];
        if (s.size() > 4 && !strcmp(s.substr(s.size() - 4).c_str(), ".exe"))    // name                                         // bug here
        {
            g_procName = s;
            std::cout << g_procName << std::endl;
            return GenerateSigByProcessName;
        }
        else   // primary key
        {
            g_key = (unsigned int)strtol(s.c_str(), &ptr, 10);
            std::cout << g_key << std::endl;
            return GenerateSigByPrimarykey;
        }
    }
    return ErrorChoice;
}

std::string trim(std::string s)
{
    int i = 0, j = s.size() - 1;
    for (; s[i] == ' ' && i <= j;) i++;
    for (; s[j] == ' ' && i <= j;) j--;
    std::string res = "";
    for (; i <= j; i++)
    {
        res += s[i];
    }
    return res;
}

bool ParseArgument(std::string preHandle_argv)
{

    std::string s = trim(preHandle_argv);
    if (!s.length()) return false;
    for (int i = 0, j = s.size() - 1; i <= j;) 
    {
        std::string t = "";
        for (; i <= j && s[i] != ' '; i++)
        {
            t += s[i];
        }
        argv.push_back(t);
        for (; i <= j && s[i] == ' '; i++);
    } 
    return true;
}

int main()
{
    // allocate console
    BOOL f = AllocConsole();
    FILE* fp;

    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    HANDLE g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTitleA("HeXas Signature Maker");
    SetConsoleTextAttribute(g_hConsole, 8);

    while (1)
    {
        SetConsoleTextAttribute(g_hConsole, 8);
        // get input
        std::string preHandle_argv = "";
        std::getline(std::cin, preHandle_argv);
        argv = {};
        if (!ParseArgument(preHandle_argv)) Info::printHelp();
        else 
        {
            // parse option
            Option choice = Parse(argv);
            if (choice == ErrorChoice)
            {
                Info::printHelp();
            }
            if (choice == GetProcList)
            {
                g_procList = {};
                GetProcessList();
                SortByDictOrder();

                for (int i = 0; i < g_procList.size(); i++) {
                    std::cout << std::dec <<  i << " - " << g_procList[i] << std::endl;
                }
                std::cout << std::endl;
                // TODO
                // Change sort algorithm
            }
            if (choice == ReGenerateSig)
            {
                GenerateSig();
            }
            if (choice == GenerateSigByProcessName)
            {
                GenerateSig();
            }
            if (choice == GenerateSigByPrimarykey)
            {
                g_procList = {};
                GetProcessList();
                SortByDictOrder();

                if (!g_procList.size()) Info::printHelp();

                g_procName = g_procList[g_key];
                GenerateSig();
            }
        }


        if (GetAsyncKeyState(VK_END))
        {
            return 0;
        }
    }

    return 0;
}