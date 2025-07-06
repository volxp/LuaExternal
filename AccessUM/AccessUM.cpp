#include "Dependencies/EnvironmentInclude.h"

#include <sstream>
#include <thread>
#include "Roblox/Compressing.h"
#include "Roblox/Execution.h"
#include "Overlay/Overlay.hpp"

using namespace Globals;
bool isInstance = false;
bool ExecutionRead = false;

// MADE BY VOLXPHY!!!!


bool RenderImgui() {
	SetConsoleTitle(L"rconsole");

	HWND robloxWindow = FindWindow(nullptr, L"Roblox");
	if (!robloxWindow) {
		std::cout << "Roblox window not found.\n";
		std::cout << "Press Enter to exit...";
		std::cin.get();
		return EXIT_FAILURE;
	}

	std::thread([&]() {
		while (true) {
			if (!FindWindow(nullptr, L"Roblox")) {
				std::cout << "Roblox closed. Exiting overlay.\n";
				exit(EXIT_SUCCESS);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		}).detach();

	SetForegroundWindow(robloxWindow);

	if (!OverlayHook->Initialize()) {
		std::cerr << "Failed to initialize overlay.\n";
		return EXIT_FAILURE;
	}
	
	return true;
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		
	}

	

}
uintptr_t GetParent(uintptr_t Addr) {
	uintptr_t Parent = Api::read_mem<uintptr_t>(Addr + offsets::Parent);

	return Parent;
}

HMODULE GetResourceModule() {
	HMODULE Module;
	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetResourceModule, &Module);
	return Module;
}

void Scripts() {

	Api::write_mem<uintptr_t>(PlayerListManager + offsets::Size, Jest);
	HMODULE module = GetResourceModule();
	if (module == NULL) {
		std::cout << "[!] FAILED ON STEP 1\n";
		std::cin.get();
		return;
	}

	HRSRC Resource = FindResourceW(module, MAKEINTRESOURCE(LuaI), MAKEINTRESOURCE(Env));
	if (Resource == NULL) {
		std::cout << "[!] FAILED ON STEP 2\n";
		std::cin.get();
		return;
	}
	HGLOBAL Data = LoadResource(module, Resource);
	if (Data == NULL) {
		std::cout << "[!] FAILED ON STEP 3\n";
		std::cin.get();
		return;
	}

	void* LockData = LockResource(Data);
	if (LockData == NULL) {
		std::cout << "[!] ERROR LOCKING RESOURCE\n";
		std::cin.get();
		return;
	}
	SInstance::SetBytecode(Jest, LuauCompress::ZstdCompress(LuauCompress::Compile("getfenv(0)['script'] = nil coroutine['wrap'](function(...)" + std::string(static_cast<char*>(LockData), SizeofResource(module, Resource)) + "\nend)() while task['wait'](9e9) do task['wait'](9e9) end")));




	while (GetForegroundWindow() != RBXhwnd) {
		SetForegroundWindow(RBXhwnd);
		Sleep(1);
	}
	SendMessageW(RBXhwnd, WM_CLOSE, NULL, NULL);

	Sleep(150);

	Api::write_mem<uintptr_t>(PlayerListManager + offsets::Size, PlayerListManager);
	Roblox::RequireBypass(ScriptContext, false);
	std::cout << "[+] Injected!\n";
	SInstance::WaitForChild(SInstance::GetChildren(CoreGui), "FinishedInit", 5);
	ExecutionRead = true;
	OverlayHook->NotifyImGui("Injected!", "Luau-Executor");

}


void Instance() {

	WorkspacePointer = SInstance::FindChild(SInstance::GetChildren(Globals::DataModel), "Workspace");
	PlayerPointer = SInstance::FindChild(SInstance::GetChildren(Globals::DataModel), "Players");
	LocalPlayer = Api::read_mem<uintptr_t>(PlayerPointer + 0x128);
	LocalCharacter = SInstance::FindChild(SInstance::GetChildren(WorkspacePointer), SInstance::GetName(LocalPlayer).c_str());
	LocalHumanoid = SInstance::FindChild(SInstance::GetChildren(LocalCharacter), "Humanoid");
	ScriptContext = SInstance::FindChild(SInstance::GetChildren(DataModel), "Script Context");

	// VR-Navigation
	PlayerScripts = SInstance::FindChild(SInstance::GetChildren(DataModel), "StarterPlayer");
	StarterPlayerScripts = SInstance::FindChild(SInstance::GetChildren(PlayerScripts), "StarterPlayerScripts");
	PlayerModule = SInstance::FindChild(SInstance::GetChildren(StarterPlayerScripts), "PlayerModule");
	ControlModule = SInstance::FindChild(SInstance::GetChildren(PlayerModule), "ControlModule");
	VrNavigation = SInstance::FindChild(SInstance::GetChildren(ControlModule), "VRNavigation");
		
	// Jest
	CorePackages = SInstance::FindChild(SInstance::GetChildren(DataModel), "CorePackages");
	Packages = SInstance::FindChild(SInstance::GetChildren(CorePackages), "Packages");
	Index = SInstance::FindChild(SInstance::GetChildren(Packages), "_Index");
	CM2D = SInstance::FindChild(SInstance::GetChildren(Index), "CollisionMatchers2D");
	CLM2D = SInstance::FindChild(SInstance::GetChildren(CM2D), "CollisionMatchers2D");
	Jest = SInstance::FindChild(SInstance::GetChildren(CLM2D), "Jest");

	// PlayerListManager
	CoreGui = SInstance::FindChild(SInstance::GetChildren(DataModel), "CoreGui");
	RobloxGui = SInstance::FindChild(SInstance::GetChildren(CoreGui), "RobloxGui");
	ModulesFolder = SInstance::FindChild(SInstance::GetChildren(RobloxGui), "Modules");
	PlayerList = SInstance::FindChild(SInstance::GetChildren(ModulesFolder), "PlayerList");
	PlayerListManager = SInstance::FindChild(SInstance::GetChildren(PlayerList), "PlayerListManager");

	char buffer[128];

	sprintf(buffer, "DataModel address: 0x%p", (void*)Globals::DataModel);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "Workspace pointer: 0x%p", (void*)WorkspacePointer);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "Player pointer: 0x%p", (void*)PlayerPointer);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "LocalPlayer: 0x%p", (void*)LocalPlayer);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "Character Pointer: 0x%p", (void*)LocalCharacter);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "Humanoid Pointer: 0x%p", (void*)LocalHumanoid);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "ScriptContext: 0x%p", (void*)ScriptContext);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "VRNavigation: 0x%p", (void*)VrNavigation);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "Packages: 0x%p", (void*)Packages);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "Index: 0x%p", (void*)Index);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "CM2D: 0x%p", (void*)CM2D);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "CLM2D: 0x%p", (void*)CLM2D);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "Jest: 0x%p", (void*)Jest);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "PlayerList: 0x%p", (void*)PlayerList);
	OverlayHook->NotifyImGui(buffer, "Instance");

	sprintf(buffer, "PlayerListManager: 0x%p", (void*)PlayerListManager);
	OverlayHook->NotifyImGui(buffer, "Instance");

	OverlayHook->NotifyImGui("TPHANDLER -> Handled Teleportion", "Instance");

	Roblox::RequireBypass(ScriptContext, true);
	Scripts();
	isInstance = true;
}



uintptr_t CachedDm;
void InjectionThread() {
	while (true) {
		Sleep(100);
		CachedDm = SInstance::GetDataModel();
		
		if (CachedDm != Globals::DataModel && SInstance::GetName(CachedDm) != "LuaApp") {
			Sleep(2300);
			ExecutionRead = false;
			Globals::DataModel = CachedDm;
			isInstance = false;
			Instance();
			continue;
		}
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	const DWORD pid = WindowsApi::getProcessId(L"RobloxPlayerBeta.exe");
	if (!pid) {
		MessageBoxA(NULL, "[-] Process not found!", "Roblox External", MB_OK | MB_ICONERROR);
		return 1;
	}

	char msg[64];
	sprintf(msg, "[+] Process found! PID: %lu", pid);
	OverlayHook->NotifyImGui(msg, "External");

	RBXhwnd = Roblox::FindWindowByPid(L"RobloxPlayerBeta.exe");

	if (!Api::attachprocess(pid)) {
		return 1;
	}

	// main thread
	std::thread(RenderImgui).detach();
	std::thread(InjectionThread).detach();

	while (true) Sleep(1000);
	return 0;
}
