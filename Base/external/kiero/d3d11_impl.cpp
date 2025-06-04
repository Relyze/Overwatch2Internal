#include "../../includes.h"
#include "../../menu/menu.h"
#include "../minhook/include/MinHook.h"

using Present = HRESULT(APIENTRY*)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flag);
using ResizeBuffers = HRESULT(APIENTRY*)(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

static Present oPresent = NULL;
static ResizeBuffers oResizeBuffers = NULL;
static bool init = false;

namespace render {
	HWND window = nullptr;
	int width = 0;
	int height = 0;
}

static void ClearRenderTarget()
{
	if (render::pRenderTargetView)
	{
		render::pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		render::pRenderTargetView->Release();
		render::pRenderTargetView = nullptr;
	}
}

static void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	render::pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	if (pBackBuffer)
	{
		render::pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &render::pRenderTargetView);
		pBackBuffer->Release();
	}
}

static void SetImGuiStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	// Window parameters
	style->WindowRounding = 10.0f;  // Adding slight rounding
	style->WindowBorderSize = 1.0f;
	style->WindowPadding = ImVec2(15, 15);
	style->WindowMinSize = ImVec2(510, 128);
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

	// Child parameters
	style->ChildRounding = 6.0f;
	style->ChildBorderSize = 1.0f;

	// Item parameters
	style->ItemSpacing = ImVec2(4, 4);
	style->ItemInnerSpacing = ImVec2(4, 4);
	style->IndentSpacing = 25.0f;

	// Frame parameters
	style->FramePadding = ImVec2(5, 3);
	style->FrameRounding = 5.0f;
	style->FrameBorderSize = 1.0f;

	// Tab parameters
	style->TabRounding = 5.0f;
	style->Colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);

	// Slider Grab Bar
	style->GrabRounding = 8.f;

	// Popup parameters
	style->PopupRounding = 10.f;

	// Colors - Dracula Palette
	style->Colors[ImGuiCol_Text] = ImVec4(0.97f, 0.97f, 0.95f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.17f, 0.21f, 0.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, 0.35f, 0.0f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.26f, 0.27f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.36f, 0.37f, 0.45f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.17f, 0.25f, 1.00f);

	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.27f, 0.27f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.17f, 0.21f, 0.75f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.27f, 0.27f, 0.35f, 0.80f);

	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.78f, 0.62f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.66f, 0.50f, 0.90f, 1.00f);

	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);

	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.48f, 0.88f, 1.00f);

	style->Colors[ImGuiCol_Button] = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.78f, 0.62f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.66f, 0.50f, 0.90f, 1.00f);

	style->Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.17f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.176f, 0.184f, 0.23f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.14f, 0.15f, 0.19f, 1.00f);

	// Making separators purple
	style->Colors[ImGuiCol_Separator] = ImVec4(0.815f, 0.058f, 0.203f, 1.f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.815f, 0.058f, 0.203f, 1.f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.815f, 0.058f, 0.203f, 1.f);
}


static long __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!render::pSwapChain)
		render::pSwapChain = pSwapChain;

	if (!init)
	{
		// Get the process window and swap chain description
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		pSwapChain->GetDesc(&swapChainDesc);
		render::window = swapChainDesc.OutputWindow;
		render::width = swapChainDesc.BufferDesc.Width;
		render::height = swapChainDesc.BufferDesc.Height;

		// Get device and context
		ID3D11Device* device = nullptr;
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device)))) {

			ID3D11DeviceContext* context = nullptr;
			device->GetImmediateContext(&context);

			// Initialize render device, context, and create the render target
			render::pDevice = device;
			render::pDeviceContext = context;

			CreateRenderTarget();

			//// Initialize window
			impl::win32::init(render::window);

			//// Initialize ImGui
			ImGui::CreateContext();
			SetImGuiStyle();

			ImGui_ImplWin32_Init(render::window);
			ImGui_ImplDX11_Init(device, context);

			//// Set up fonts
			ImFontConfig cfg;
			ImGuiIO& io = ImGui::GetIO();
			io.LogFilename = nullptr;
			io.IniFilename = nullptr;

			io.FontDefault = io.Fonts->AddFontFromMemoryCompressedBase85TTF(segoe_font_compressed_data_base85, 20.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
			io.Fonts->AddFontFromMemoryCompressedBase85TTF(overwatch_font_compressed_data_base85, 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
			
			init = true;
		}
		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	// Setup and render ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// If in game, render visuals
	if (update::isIngame) {
		if (config->DrawFOV)
			drawFOV();
		features::doVisuals();
	}

	menu::Render(globals::g_MenuState);

	// End ImGui frame and render
	ImGui::Render();
	render::pDeviceContext->OMSetRenderTargets(1, &render::pRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Call original present function
	return oPresent(pSwapChain, SyncInterval, Flags);
}

static HRESULT hkResizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
{
	// Invalidate and clear
	ClearRenderTarget();

	// Call original function
	HRESULT hr = oResizeBuffers(swapChain, bufferCount, width, height, newFormat, swapChainFlags);

	// Get swap chain description and update width and height
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	render::pSwapChain->GetDesc(&swapChainDesc);
	render::window = swapChainDesc.OutputWindow;
	render::width = swapChainDesc.BufferDesc.Width;
	render::height = swapChainDesc.BufferDesc.Height;

	// Recreate and create
	CreateRenderTarget();
	render::pDeviceContext->OMSetRenderTargets(1, &render::pRenderTargetView, nullptr);

	D3D11_VIEWPORT vp{};
	vp.Width = render::width;
	vp.Height = render::height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	render::pDeviceContext->RSSetViewports(1, &vp);

	return hr;
}

void impl::d3d11::init()
{
	if (!kiero::bind(8, (void**)&oPresent, hkPresent11) == kiero::Status::Success) {
		if (globals::g_Debug) {
			printf(enc("Failed to hook present!\n").c_str());
		}
	}
	if (!kiero::bind(13, (void**)&oResizeBuffers, hkResizeBuffers) == kiero::Status::Success) {
		if (globals::g_Debug) {
			printf(enc("ResizeBuffers Hooked!\n").c_str());
		}
	}
}