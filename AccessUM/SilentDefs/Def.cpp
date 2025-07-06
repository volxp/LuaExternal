#include "Def.hpp"
#include "../Driver/DriverManager.hpp"
#include <thread>
#include <chrono>


using namespace Globals;

uintptr_t SInstance::FindChild(uintptr_t childrenVecAddr, const std::string& name) {
	constexpr int nameLength = 640;
	uintptr_t top = Api::read_mem<uintptr_t>(childrenVecAddr);
	uintptr_t end = Api::read_mem<uintptr_t>(childrenVecAddr + 0x8) + 1;
	for (uintptr_t childPtr = top; childPtr < end; childPtr += 0x10) {
		uintptr_t childAddr = Api::read_mem<uintptr_t>(childPtr);
		if (childAddr == 0)
			break;

		uintptr_t namePtr = Api::read_mem<uintptr_t>(childAddr + offsets::Name);
		std::string nameBuffer = Roblox::rbx_string(namePtr);

		if (nameBuffer == name) {
			return childAddr;
		}
	}

	return 0;
}

uintptr_t SInstance::GetDataModel() {
	uintptr_t FakeDataModel = Api::read_mem<uintptr_t>(offsets::FakeDatamodelPointer);
	uintptr_t Datamodel = Api::read_mem<uintptr_t>(FakeDataModel + offsets::DataModelPointer);
	if (!Datamodel) {
		std::cout << "[-] Failed to get DataModel!" << std::endl;
		return 0x0;
	}
	return Datamodel;
}

std::string SInstance::GetName(uintptr_t addr) {
	uintptr_t namePtr = Api::read_mem<uintptr_t>(addr + offsets::Name);
	if (namePtr == 0)
		return "";

	std::string result;
	char c;
	do {
		c = Api::read_mem<char>(namePtr + result.length());
		if (c != '\0')
			result += c;
	} while (c != '\0' && result.length() < 64);

	return result;
}

std::string SInstance::ClassName(uintptr_t addr) {
	uintptr_t ClassDescriptor = Api::read_mem<uint64_t>(addr + offsets::ClassDescriptor);
	uintptr_t ClassNamePtr = Api::read_mem<uint64_t>(ClassDescriptor + 0x8);

	if (ClassNamePtr) {
		uintptr_t Size = Api::read_mem<size_t>(ClassNamePtr + 0x10);

		if (Size >= 16)
			ClassNamePtr = Api::read_mem<uintptr_t>(ClassNamePtr);

		std::string Final;
		BYTE Code = 0;
		for (std::int32_t Index = 0; Code = Api::read_mem<uint8_t>(ClassNamePtr + Index); Index++) 
			Final.push_back(Code);
		
		return Final;
	}

	return "Class Name not found!";
}

//uintptr_t SInstance::GetJob(const char* name) {
//	constexpr int maxJobs = 256;
//	constexpr int nameLength = 64;
//	uintptr_t rawScheduler = Api::read_mem<uintptr_t>(offsets::RawScheduler);
//	uintptr_t jobListBase = Api::read_mem<uintptr_t>(rawScheduler + offsets::JobStart);
//	for (int i = 0; i < maxJobs; i++) {
//		uintptr_t jobPtrAddr = jobListBase + i * sizeof(uintptr_t);
//		uintptr_t jobPtr = Api::read_mem<uintptr_t>(jobPtrAddr);
//		if (jobPtr == 0)
//			continue;
//		char nameBuffer[nameLength] = {};
//		for (int j = 0; j < nameLength - 1; j++) {
//			char c = Api::read_mem<char>(jobPtr + offsets::JobName + j);
//			if (c == '\0') break;
//			nameBuffer[j] = c;
//		}
//
//		if (strlen(nameBuffer) == 0)
//			continue;
//		if (strstr(nameBuffer, name)) {
//			return jobPtr;
//		}
//	}
//	return 0;
//}

uintptr_t SInstance::GetChildren(uintptr_t addr) {
	return Api::read_mem<uintptr_t>(addr + offsets::Children);
}


uintptr_t SInstance::GetParent(uintptr_t Addr) {
	uintptr_t Parent = Api::read_mem<uintptr_t>(Addr + offsets::Parent);

	return Parent;
}

void SInstance::SetBytecode(uintptr_t addr, std::string Bytecode) {
	if (ClassName(addr) == "ModuleScript") {
		uintptr_t ModuleScriptBytecodePtr = Api::read_mem<uintptr_t>(addr + offsets::ModuleScriptEmbedded);

		uintptr_t OldBytecode = Api::read_mem<uintptr_t>(ModuleScriptBytecodePtr + 0x10);
		uintptr_t OldBytecodeSize = Api::read_mem<uintptr_t>(ModuleScriptBytecodePtr + 0x20);

		std::thread([ModuleScriptBytecodePtr, OldBytecode, OldBytecodeSize]() {
			Sleep(1000);
			Api::write_mem<uintptr_t>(ModuleScriptBytecodePtr + 0x10, OldBytecode);
			Api::write_mem<uintptr_t>(ModuleScriptBytecodePtr + 0x20, OldBytecodeSize);
			}).detach();

		for (size_t i = 0; i < Bytecode.size(); i++) {
			Api::write_mem<BYTE>(OldBytecode + i, static_cast<BYTE>(Bytecode[i]));
		}
		Api::write_mem<uintptr_t>(ModuleScriptBytecodePtr + 0x20, Bytecode.size());
	}
}


uintptr_t SInstance::ObjectValue(uintptr_t addr) {
	return Api::read_mem<uintptr_t>(addr + offsets::value);
}

uintptr_t SInstance::WaitForChild(uintptr_t Starter, std::string InstanceName, int MaxTime) {
	std::chrono::steady_clock::time_point Start = std::chrono::high_resolution_clock::now();
	auto Timeout = std::chrono::seconds(MaxTime);

	while (std::chrono::steady_clock::now() - Start <= Timeout) {
		uintptr_t Found = FindChild(Starter, InstanceName);
		if (Found)
			return Found;

		Sleep(100); // gib dem Ding Zeit
	}

	return 0; // nicht gefunden innerhalb der Zeit
}



bool Roblox::RequireBypass(uintptr_t ScriptContext, bool type) {

	int val;
	if (type == true) {
		val = 1;
	}
	else {
		val = 0;
	}

	Api::write_mem<uint32_t>(ScriptContext + offsets::RequireBypass, val);
	if (Api::read_mem<uint32_t>(ScriptContext + offsets::RequireBypass) == val) {
		std::cout << "[+] (ScriptContext -> RequireBypass): Done.\n";
		return true;
	}
	else {
		return false;
	}
}

std::string Roblox::rbx_string(uintptr_t addr) {
	int32_t check = Api::read_mem<int32_t>(addr + 0x18);
	if (check > 15)
		addr = Api::read_mem<uintptr_t>(addr);

	std::string name = "";
	for (int j = 0; j < 640; j++) {
		char c = Api::read_mem<char>(addr + j);
		if (c == '\0') break;
		name.push_back(c);
	}

	return name;
}

bool Roblox::CoreBypass(uintptr_t addr) {
	Api::write_mem<uintptr_t>(addr + offsets::ModuleFlags, 0x100000000);
	Api::write_mem<uintptr_t>(addr + offsets::IsCoreScript, 0x1);
	return true;
}


HWND Roblox::FindWindowByPid(const std::wstring& processName) {
	HWND hwnd = nullptr;

	// Take snapshot of all processes
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) return nullptr;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	DWORD pid = 0;
	if (Process32First(hSnap, &pe)) {
		do {
			if (!_wcsicmp(pe.szExeFile, processName.c_str())) {
				pid = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);

	if (pid == 0) return nullptr;

	// Find window with matching PID
	struct HandleData {
		DWORD pid;
		HWND hwnd;
	} data = { pid, nullptr };

	auto EnumWindowsCallback = [](HWND hwnd, LPARAM lParam) -> BOOL {
		HandleData* pData = (HandleData*)lParam;
		DWORD winPID;
		GetWindowThreadProcessId(hwnd, &winPID);
		if (winPID == pData->pid && GetWindow(hwnd, GW_OWNER) == nullptr && IsWindowVisible(hwnd)) {
			pData->hwnd = hwnd;
			return FALSE; // Stop enumeration
		}
		return TRUE;
		};

	EnumWindows(EnumWindowsCallback, (LPARAM)&data);
	return data.hwnd;
}