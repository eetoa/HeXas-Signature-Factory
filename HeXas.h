

extern HANDLE g_hConsole;
extern std::string g_procName;
extern uintptr_t g_address;
extern unsigned int g_key;
extern uintptr_t g_size;
extern HANDLE	g_procHandle;
extern DWORD	g_procId;
extern std::vector<std::string> argv;
extern std::vector<std::string> g_procList;


enum Option
{
	ShowHelp = 0,
	ListProgress = 1,
	ListGroup = 2,
	NewGroup = 3,
	ShowGroup = 4,
	MergeGroup = 5,
	GetGroup = 6,
	ShowAllGroup = 7,
	MergeAllGroup = 8,
	GetAllGroup = 9,
	ReGenerateSig = 10,
	GenerateSigByProcessName = 11,
	GenerateSigByPrimarykey = 12,
	ErrorOption = -1,
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

static void DrawHeader()
{
	using namespace std;

	cout << "                                                                                                " << endl;
	cout << "                                                                                                " << endl;
	cout << "HHHHHHHHH     HHHHHHHHH                  XXXXXXX       XXXXXXX                                  " << endl;
	cout << "H:::::::H     H:::::::H                  X:::::X       X:::::X                                  " << endl;
	cout << "H:::::::H     H:::::::H                  X:::::X       X:::::X                                  " << endl;
	cout << "HH::::::H     H::::::HH                  X::::::X     X::::::X                                  " << endl;
	cout << "  H:::::H     H:::::H      eeeeeeeeeeee  XXX:::::X   X:::::XXX  aaaaaaaaaaaaa      ssssssssss   " << endl;
	cout << "  H:::::H     H:::::H    ee::::::::::::ee   X:::::X X:::::X     a::::::::::::a   ss::::::::::s  " << endl;
	cout << "  H::::::HHHHH::::::H   e::::::eeeee:::::ee  X:::::X:::::X      aaaaaaaaa:::::ass:::::::::::::s " << endl;
	cout << "  H:::::::::::::::::H  e::::::e     e:::::e   X:::::::::X                a::::as::::::ssss:::::s" << endl;
	cout << "  H:::::::::::::::::H  e:::::::eeeee::::::e   X:::::::::X         aaaaaaa:::::a s:::::s  ssssss " << endl;
	cout << "  H::::::HHHHH::::::H  e:::::::::::::::::e   X:::::X:::::X      aa::::::::::::a   s::::::s      " << endl;
	cout << "  H:::::H     H:::::H  e::::::eeeeeeeeeee   X:::::X X:::::X    a::::aaaa::::::a      s::::::s   " << endl;
	cout << "  H:::::H     H:::::H  e:::::::e         XXX:::::X   X:::::XXXa::::a    a:::::assssss   s:::::s " << endl;
	cout << "HH::::::H     H::::::HHe::::::::e        X::::::X     X::::::Xa::::a    a:::::as:::::ssss::::::s" << endl;
	cout << "H:::::::H     H:::::::H e::::::::eeeeeeeeX:::::X       X:::::Xa:::::aaaa::::::as::::::::::::::s " << endl;
	cout << "H:::::::H     H:::::::H  ee:::::::::::::eX:::::X       X:::::X a::::::::::aa:::as:::::::::::ss  " << endl;
	cout << "HHHHHHHHH     HHHHHHHHH    eeeeeeeeeeeeeeXXXXXXX       XXXXXXX  aaaaaaaaaa  aaaa sssssssssss    " << endl;
	cout << "                                                                                                " << endl;
	cout << "                                                                                                " << endl;

}