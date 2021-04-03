

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