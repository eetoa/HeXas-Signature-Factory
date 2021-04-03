#include "stdafx.h"

HANDLE g_hConsole;
std::string g_procName;
uintptr_t g_address;
unsigned int g_key;
uintptr_t g_size;
HANDLE	g_procHandle;
DWORD	g_procId;
std::vector<std::string> argv;
std::vector<std::string> g_procList;



Option Parse(std::vector<std::string> cmd)
{
    int len = cmd.size();
    if (len != 2 && len != 4 && len != 7) return ErrorOption;

    if (len == 2 && !strcmp(cmd[0].c_str(), "ls") && !strcmp(cmd[1].c_str(), "-p"))         // ls progresses
    {
        return ListProgress;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "ls") && !strcmp(cmd[1].c_str(), "-g"))         // ls groups
    {
        return ListGroup;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "new"))                                         // new group
    {
        return NewGroup;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "show"))                                        // show group
    {
        return ShowGroup;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "merge"))                                       // merge group
    {
        return MergeGroup;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "get"))                                         // get final sig from group
    {
        return GetGroup;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "show") && !strcmp(cmd[1].c_str(), "-all"))
    {
        return ShowAllGroup;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "merge") && !strcmp(cmd[1].c_str(), "-all"))
    {
        return MergeAllGroup;
    }
    if (len == 2 && !strcmp(cmd[0].c_str(), "get") && !strcmp(cmd[1].c_str(), "-all"))
    {
        return GetAllGroup;
    }
    if (len == 4 && !strcmp(cmd[1].c_str(), "-r") && !strcmp(cmd[2].c_str(), "-a"))         // repeat get sig with last info
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
    return ErrorOption;
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
            if (choice == ErrorOption)
            {
                Info::printHelp();
            }
            if (choice == ListProgress)
            {
                g_procList = {};
                Proc::GetProcessList();
                Utils::SortByDictOrder();

                for (int i = 0; i < g_procList.size(); i++) {
                    std::cout << std::dec <<  i << " - " << g_procList[i] << std::endl;
                }
                std::cout << std::endl;
                // TODO
                // Change sort algorithm
            }
            if (choice == ListGroup)
            {
                Factory::ListGroup();
            }
            if (choice == NewGroup)
            {
                Factory::AddGroup(argv[1]);
            }
            if (choice == ShowGroup)
            {
                Factory::ShowGroup(argv[1]);
            }
            if (choice == MergeGroup)
            {
                Factory::MergeGroup(argv[1]);
            }
            if (choice == GetGroup)
            {
                Factory::GetProduct(argv[1]);
            }
            if (choice == ShowAllGroup)
            {
                Factory::ShowAllGroup();
            }
            if (choice == MergeAllGroup)
            {
                Factory::MergeAllGroup();
            }
            if (choice == GetAllGroup)
            {
                Factory::GetAllProduct();
            }
            if (choice == ReGenerateSig)
            {
                Factory::GenerateSig(argv[0]);
            }
            if (choice == GenerateSigByProcessName)
            {
                Factory::GenerateSig(argv[0]);
            }
            if (choice == GenerateSigByPrimarykey)
            {
                g_procList = {};
                Proc::GetProcessList();
                Utils::SortByDictOrder();

                if (!g_procList.size()) Info::printHelp();

                g_procName = g_procList[g_key];
                Factory::GenerateSig(argv[0]);
            }
        }


        if (GetAsyncKeyState(VK_END))
        {
            return 0;
        }
    }

    return 0;
}