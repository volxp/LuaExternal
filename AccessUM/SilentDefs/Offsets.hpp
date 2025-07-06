#pragma once
#include "../WindowsApi/Manager.hpp"

using namespace WindowsApi;

#define REBASE(x) x + GetModuleBaseAddress(getProcessId(L"RobloxPlayerBeta.exe"), L"RobloxPlayerBeta.exe")

namespace Globals {
	inline uintptr_t DataModel;
	inline HANDLE processhandle;
	inline HWND RBXhwnd;

	// pointers
	inline uintptr_t HeartBeat = 0x0;
	inline uintptr_t PacketJ = 0x0;
	inline uintptr_t WorkspacePointer = 0x0;
	inline uintptr_t PlayerPointer = 0x0;
	inline uintptr_t LocalPlayer = 0x0;
	inline uintptr_t LocalCharacter = 0x0;
	inline uintptr_t LocalHumanoid = 0x0;
	inline uintptr_t ScriptContext = 0x0;
	inline uintptr_t VrNavigation = 0x0;
	inline uintptr_t PlayerScripts = 0x0;
	inline uintptr_t PlayerModule = 0x0;
	inline uintptr_t ControlModule = 0x0;
	inline uintptr_t StarterPlayerScripts = 0x0;
	inline uintptr_t Packages = 0x0;
	inline uintptr_t Index = 0x0;
	inline uintptr_t CM2D = 0x0;
	inline uintptr_t CLM2D = 0x0;
	inline uintptr_t Jest = 0x0;
	inline uintptr_t CorePackages = 0x0;

	inline uintptr_t CoreGui = 0x0;
	inline uintptr_t RobloxGui = 0x0;
	inline uintptr_t ModulesFolder = 0x0;
	inline uintptr_t PlayerList = 0x0;
	inline uintptr_t PlayerListManager = 0x0;
}
namespace offsets {
	inline uintptr_t FakeDatamodelPointer = REBASE(0x67D0C18); // gets outdated every version!


	// doesnt really change...
	inline uintptr_t DataModelPointer = 0x1B8;
	inline uintptr_t RequireBypass = 0x6D8;
	inline uintptr_t ClassDescriptor = 0x18;
	inline uintptr_t Children = 0x80;
	inline uintptr_t Name = 0x78;
	inline uintptr_t Parent = 0x50;
	inline uintptr_t Size = 0x8;
	inline uintptr_t IsCoreScript = 0x1a0;
	inline uintptr_t ModuleFlags = IsCoreScript - 0x4;
	inline uintptr_t value = 0xD8;
	inline uintptr_t ModuleScriptEmbedded = 0x158;

	// later for rendering....
	//inline uintptr_t RawScheduler = REBASE(0x6897128);
	//inline uintptr_t JobEnd = 0x1D8;
	//inline uintptr_t JobStart = 0x1D0;
	//inline uintptr_t JobName = 0x18;
	//inline uintptr_t JobId = 0x140;

	
}