#include "Overlay.hpp"
#include "../Roblox/Execution.h"
#include "../Driver/DriverManager.hpp"

static TextEditor Editor;
float jpnew = 50.0f;
float walkspeed = 16.0f;
void SetImGuiCustomTheme()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	// General style settings
	style->WindowRounding = 5.0f;
	style->FrameRounding = 5.0f;
	style->GrabRounding = 5.0f;
	style->ScrollbarRounding = 5.0f;
	style->FrameBorderSize = 1.0f;
	style->WindowBorderSize = 1.0f;
	style->ItemSpacing = ImVec2(8, 6);
	style->WindowPadding = ImVec2(12, 12);

	// Set custom colors
	colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);           // Dark background
	colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);            // Editor area bg
	colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);             // Borders
	colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);            // Text area bg
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

	colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);             // Button color
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);

	colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);                // Tab inactive
	colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);

	colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);            // Title Bar
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);

	colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);               // Main text
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

	colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.30f, 0.30f, 1.00f);          // Red Checkmark

	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

	// Optional: Change highlight color to the red hue in the code editor
	colors[ImGuiCol_Header] = ImVec4(0.25f, 0.10f, 0.10f, 1.00f);             // For selected items
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.20f, 0.20f, 1.00f);
}

void COverlayHook::NotifyImGui(std::string message, std::string title)
{
	// Add new notification to the list
	notifications.emplace_back(message, title, 4.0f);

	// Limit to maximum 5 notifications to prevent screen clutter
	if (notifications.size() > 5)
	{
		notifications.erase(notifications.begin());
	}
}

void COverlayHook::RenderNotifications()
{
	// Clean up expired notifications
	notifications.erase(
		std::remove_if(notifications.begin(), notifications.end(),
			[](const Notification& n) { return n.IsExpired(); }),
		notifications.end()
	);

	// Get screen dimensions
	ImVec2 screenSize = ImGui::GetIO().DisplaySize;
	const float notificationWidth = 300.0f;
	const float notificationHeight = 80.0f;
	const float margin = 10.0f;

	// Render notifications from bottom to top
	for (size_t i = 0; i < notifications.size(); ++i)
	{
		const auto& notification = notifications[i];

		// Calculate position (bottom-right corner)
		float yPos = screenSize.y - (notificationHeight + margin) * (i + 1);
		float xPos = screenSize.x - notificationWidth - margin;

		// Set next window position and size
		ImGui::SetNextWindowPos(ImVec2(xPos, yPos));
		ImGui::SetNextWindowSize(ImVec2(notificationWidth, notificationHeight));

		// Apply fade effect
		float alpha = notification.GetAlpha();
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

		// Create notification window
		std::string windowName = "##Notification" + std::to_string(i);
		ImGui::Begin(windowName.c_str(), nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_AlwaysAutoResize);

		// Title
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
		ImGui::TextWrapped("%s", notification.title.c_str());
		ImGui::PopStyleColor();

		ImGui::Separator();

		// Message
		ImGui::TextWrapped("%s", notification.message.c_str());

		ImGui::End();
		ImGui::PopStyleVar();
	}
}

bool COverlayHook::CreateD3DDevice(HWND handle)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	RtlZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = handle;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const UINT CreateDeviceFlags = 0;
	D3D_FEATURE_LEVEL D3DFeatureLevel;
	const D3D_FEATURE_LEVEL FeatureLevelArr[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT Result = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceFlags, FeatureLevelArr,
		2, D3D11_SDK_VERSION, &SwapChainDesc, &this->DxgiSwapChain, &this->D3D11Device,
		&D3DFeatureLevel, &this->D3D11DeviceContext);

	if (Result == DXGI_ERROR_UNSUPPORTED)
	{
		Result = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_WARP, nullptr, CreateDeviceFlags, FeatureLevelArr,
			2, D3D11_SDK_VERSION, &SwapChainDesc, &this->DxgiSwapChain, &this->D3D11Device,
			&D3DFeatureLevel, &this->D3D11DeviceContext);
	}

	if (Result != S_OK)
		return false;

	this->CreateRenderTarget();
	return true;
}

void COverlayHook::CleanupD3DDevice()
{
	this->CleanupRenderTarget();

	if (this->DxgiSwapChain)
	{
		this->DxgiSwapChain->Release();
		this->DxgiSwapChain = nullptr;
	}

	if (this->D3D11DeviceContext)
	{
		this->D3D11DeviceContext->Release();
		this->D3D11DeviceContext = nullptr;
	}

	if (this->D3D11Device)
	{
		this->D3D11Device->Release();
		this->D3D11Device = nullptr;
	}
}

void COverlayHook::CreateRenderTarget()
{
	ID3D11Texture2D* D3D11BackBuffer;
	this->DxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&D3D11BackBuffer));
	if (D3D11BackBuffer != nullptr)
	{
		this->D3D11Device->CreateRenderTargetView(D3D11BackBuffer, nullptr, &this->D3D11RenderTargetView);
		D3D11BackBuffer->Release();
	}
}

void COverlayHook::CleanupRenderTarget()
{
	if (this->D3D11RenderTargetView)
	{
		this->D3D11RenderTargetView->Release();
		this->D3D11RenderTargetView = nullptr;
	}
}

__forceinline LRESULT __stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (OverlayHook->D3D11Device != nullptr && wParam != SIZE_MINIMIZED)
		{
			OverlayHook->CleanupRenderTarget();
			OverlayHook->DxgiSwapChain->ResizeBuffers(2, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			OverlayHook->CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

bool COverlayHook::Initialize()
{
	Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	Editor.SetText("print(\"Hello world!\")");

	ImGui_ImplWin32_EnableDpiAwareness();

	WNDCLASSEX WindowClass;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.cbWndExtra = 0;
	WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(CreateSolidBrush(RGB(0, 0, 0)));
	WindowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	WindowClass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
	WindowClass.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);
	WindowClass.hInstance = GetModuleHandleA(nullptr);
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.lpszClassName = L"Execution";
	WindowClass.lpszMenuName = nullptr;
	WindowClass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassExW(&WindowClass);
	const auto Window = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, WindowClass.lpszClassName, L"Execution",
		WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, WindowClass.hInstance, nullptr);

	SetLayeredWindowAttributes(Window, 0, 255, LWA_ALPHA);

	const MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(Window, &Margin);

	if (!this->CreateD3DDevice(Window))
	{
		this->CleanupD3DDevice();
		UnregisterClassW(WindowClass.lpszClassName, WindowClass.hInstance);
		return false;
	}

	ShowWindow(Window, SW_SHOW);
	UpdateWindow(Window);

	ImGui::CreateContext();
	ImGui::GetIO().IniFilename = nullptr;

	SetImGuiCustomTheme();

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX11_Init(this->D3D11Device, this->D3D11DeviceContext);

	const ImVec4 ClearColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	bool Init = true, Draw = true, Done = false;
	while (!Done)
	{
		MSG Message;
		while (PeekMessageA(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessageA(&Message);
			if (Message.message == WM_QUIT)
				Done = true;
		}

		if (Done)
			break;

		const auto TargetWindow = FindWindow(nullptr, L"Roblox");
		const auto ForegroundWindow = GetForegroundWindow();

		if (TargetWindow != ForegroundWindow && Window != ForegroundWindow)
			MoveWindow(Window, 0, 0, 0, 0, true);
		else
		{
			RECT Rect;
			GetWindowRect(TargetWindow, &Rect);

			auto RSizeX = Rect.right - Rect.left;
			auto RSizeY = Rect.bottom - Rect.top;

			auto Status = false;
			MONITORINFO MonitorInfo = { sizeof(MONITORINFO) };
			if (GetMonitorInfoA(MonitorFromWindow(TargetWindow, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
			{
				RECT WindowRect;
				if (GetWindowRect(TargetWindow, &WindowRect))
				{
					Status = WindowRect.left == MonitorInfo.rcMonitor.left
						&& WindowRect.right == MonitorInfo.rcMonitor.right
						&& WindowRect.top == MonitorInfo.rcMonitor.top
						&& WindowRect.bottom == MonitorInfo.rcMonitor.bottom;
				}
			}

			if (Status)
			{
				RSizeX += 16;
				RSizeY -= 24;
			}
			else
			{
				RSizeY -= 63;
				Rect.left += 8;
				Rect.top += 31;
			}
			MoveWindow(Window, Rect.left, Rect.top, RSizeX, RSizeY, 1);
		}

		if ((GetAsyncKeyState(VK_INSERT) & 1))
			Draw = !Draw;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			if (GetForegroundWindow() == FindWindowA(nullptr, "Roblox") || GetForegroundWindow() == Window)
			{
				if (Draw)
				{
					ImGui::SetNextWindowSize(ImVec2(600, 355));
					ImGui::Begin("Executor", nullptr, ImGuiWindowFlags_NoResize);
					{
						if (ImGui::BeginTabBar("#Tabs", ImGuiTabBarFlags_None))
						{
							if (ImGui::BeginTabItem("Execution"))
							{
								if (ImGui::Button("Execute"))
									std::thread([] {
									ExecutionService::Execute(Editor.GetText());
										}).detach();

								ImGui::SameLine();

								if (ImGui::Button("Clear"))
									Editor.SetText("");

								Editor.Render("TextEditor");
								ImGui::Separator();
								ImGui::EndTabItem();
							}

							if (ImGui::BeginTabItem("Player"))
							{
								ImGui::Text("Player | UD");
								ImGui::Separator();
								ImGui::Spacing();

								

								if (ImGui::SliderFloat("Jump Power", &jpnew, 0, 1000)) {
									Api::write_mem<float>(LocalHumanoid + 0x1B8, jpnew);
								}
								if (ImGui::SliderFloat("Walkspeed", &walkspeed, 0, 1000)) {
									Api::write_mem<float>(LocalHumanoid + 0x1D8, walkspeed);
									Api::write_mem<float>(LocalHumanoid + 0x3B0, walkspeed);
								}
								


								ImGui::SameLine();
								ImGui::EndTabItem();
							}

							ImGui::EndTabBar();
						}
					}
					ImGui::End();
				}
			}

			OverlayHook->RenderNotifications();

			if (Draw)
				SetWindowLongA(Window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
			else
				SetWindowLongA(Window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

			ImGui::EndFrame();
			ImGui::Render();

			const float ClearColorWithAlpha[4] = { ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w };
			this->D3D11DeviceContext->OMSetRenderTargets(1, &this->D3D11RenderTargetView, nullptr);
			this->D3D11DeviceContext->ClearRenderTargetView(this->D3D11RenderTargetView, ClearColorWithAlpha);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			this->DxgiSwapChain->Present(1, 0);

			const float targetFrameTime = 1.0f / 120.0f;
			static DWORD lastTime = timeGetTime();
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - lastTime) / 1000.0f;

			if (deltaTime < targetFrameTime)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((targetFrameTime - deltaTime) * 1000)));
			}

			lastTime = timeGetTime();
		}
	}

	Init = false;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	this->CleanupD3DDevice();
	DestroyWindow(Window);
	UnregisterClassW(WindowClass.lpszClassName, WindowClass.hInstance);
}