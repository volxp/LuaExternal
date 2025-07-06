#include "Manager.hpp"


DWORD WindowsApi::getProcessId(const wchar_t* processname) {
	PROCESSENTRY32W entry = { sizeof(entry) };
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) return 0;

	while (Process32NextW(snapshot, &entry)) {
		if (_wcsicmp(entry.szExeFile, processname) == 0) {
			CloseHandle(snapshot);
			return entry.th32ProcessID;
		}
	}
	CloseHandle(snapshot);
	return 0;
}

void WindowsApi::WriteMemory(HANDLE process, PVOID target, PVOID buffer, SIZE_T size, SIZE_T* returnSize) {
	returnSize = nullptr;
	WriteProcessMemory(process, target, buffer, size, returnSize);
}

uintptr_t WindowsApi::GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName) {
    uintptr_t moduleBase = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32W);
        if (Module32FirstW(snapshot, &moduleEntry)) {
            do {
                if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                    moduleBase = (uintptr_t)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(snapshot, &moduleEntry));
        }
        CloseHandle(snapshot);
    }
    return moduleBase;
}