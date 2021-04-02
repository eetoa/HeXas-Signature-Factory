#include "HeXas.h"


namespace Info {

    using namespace std;
    inline void printHelp()
    {
        cout << "Help Information" << endl;
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
    const wchar_t* procName = CharToWchar(g_procName);
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
    std::cout << g_procId << " - " << g_procHandle;

    std::vector<byte> res;
    for (unsigned int i = 0; i < g_size; i++) {
        byte cur;
        res.push_back(ReadEx<byte>(g_address + i));
    }

    for (int i = 0; i < res.size(); i++) {
        std::cout << std::hex << (unsigned int)res[i] << std::endl;
    }

}

Option Parse(int argc, char* argv[])
{
    if (argc != 2 && argc != 4 && argc != 7) return ErrorChoice;
    //std::cout << argc << std::endl;

    if (argc == 2 && !strcmp(argv[1], "-ls"))   // hexas -ls
    {
        return GetProcList;
    }

    if (argc == 4 && !strcmp(argv[1], "-r") && !strcmp(argv[2], "-a"))  // hexas -r -a 
    {
        long add;
        char* ptr;
        add = strtol(argv[3], &ptr, 16);
        g_address = (uintptr_t)add;
        return ReGenerateSig;
    }
    /*
    for (int i = 0; i < argc; i++) {
        std::cout << i << " - " << argv[i] << std::endl;
    }
    */
    if (argc == 7 && !strcmp(argv[1], "-p") && !strcmp(argv[3], "-a") && !strcmp(argv[5], "-s")) {   // hexas -p [name or key] -a [address] -s [size]
        //std::cout << argc << std::endl;

        char* ptr;

        // parse address
        long add;
        add = strtol(argv[4], &ptr, 16);
        g_address = (uintptr_t)add;
        std::cout << std::hex << g_address << std::endl;

        // parse size
        add = strtol(argv[6], &ptr, 16);
        g_size = (uintptr_t)add;
        std::cout << std::hex << g_size << std::endl;

        // determin name or key
        std::string s;
        s = argv[2];
        if (!strcmp(s.substr(s.size() - 4).c_str(), ".exe"))    // name                                         // bug here
        {
            const char* cc = argv[2];
            //g_procName = (const wchar_t*)(cc);
            std::cout << (const char*)g_procName << std::endl;
            return GenerateSigByProcessName;
        }
        else   // primary key
        {
            g_key = (unsigned int)strtol(argv[2], &ptr, 10);
            return GenerateSigByPrimarykey;
        }
    }
    return ErrorChoice;
}

int main(int argc, char* argv[])
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

    //GenerateSig();

    {
        Option choice = Parse(argc, argv);
        if (choice == ErrorChoice)
        {
            Info::printHelp();
            return 0;
        }
        if (choice == GetProcList)
        {
            GetProcessList();
            SortByDictOrder();

            for (int i = 0; i < g_procList.size(); i++) {
                std::cout << g_procList[i] << std::endl;
            }
            // TODO
            // Change sort algorithm
            // write to local
            return 0;
        }
        if (choice == ReGenerateSig)
        {
            // reed last choice info from local
            // GetSig
        }
        if (choice == GenerateSigByProcessName)
        {
            GenerateSig();
        }
        if (choice == GenerateSigByPrimarykey)
        {
            //read process name from local list
            // determin if exists list in local
            // Getsig
        }
    }

    return 0;
}