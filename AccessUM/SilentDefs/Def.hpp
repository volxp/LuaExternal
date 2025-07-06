#include "../SilentDefs/Offsets.hpp"

#include <iostream>

namespace SInstance {
	uintptr_t GetDataModel();

	uintptr_t GetChildren(uintptr_t Addr);
	std::string ClassName(uintptr_t addr);
	uintptr_t GetParent(uintptr_t Addr);
	uintptr_t FindChild(uintptr_t childrenVecAddr, const std::string& name);
	std::string GetName(uintptr_t addr);
	uintptr_t ObjectValue(uintptr_t val);
	void SetBytecode(uintptr_t addr, std::string Bytecode);
	uintptr_t WaitForChild(uintptr_t Starter, std::string InstanceName, int MaxTime);

}
namespace Roblox {
	std::string rbx_string(uintptr_t addr);
	bool RequireBypass(uintptr_t ScriptContext, bool type);
	bool CoreBypass(uintptr_t addr);

	HWND FindWindowByPid(const std::wstring& processName);
}


