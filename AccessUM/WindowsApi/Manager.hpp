#include <Windows.h>
#include <TlHelp32.h>

namespace WindowsApi {
	DWORD getProcessId(const wchar_t* processname);
	void WriteMemory(HANDLE processId, PVOID target, PVOID buffer, SIZE_T size, SIZE_T* returnSize);
	uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName);
}