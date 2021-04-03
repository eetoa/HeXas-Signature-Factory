
extern HANDLE g_hConsole;
extern std::string g_procName;
extern uintptr_t g_address;
extern unsigned int g_key;
extern uintptr_t g_size;
extern HANDLE	g_procHandle;
extern DWORD	g_procId;
extern std::vector<std::string> argv;
extern std::vector<std::string> g_procList;

namespace Info {

	using namespace std;
	inline void printHelp()
	{
		cout << "Help Information" << endl << endl;
	}
}

enum Option
{
	GetProcList = 1,
	NewGroup = 2,
	MergeGroup = 3,
	GetGroup = 4,
	ReGenerateSig = 5,
	GenerateSigByProcessName = 6,
	GenerateSigByPrimarykey = 7,
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