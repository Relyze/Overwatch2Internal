#pragma once

#define WIN32_LEAN_AND_MEAN

// ==================== Standard Libraries ====================
#define NOMINMAX
#include <windows.h>
#include <winternl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <TlHelp32.h>

#include <type_traits>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>

// ==================== External Utilities ====================
#include "external/include/skcrypter.h"
#include "external/include/json.hpp"
#include "external/include/idadefs.h"

using json = nlohmann::json;
//static json diagnosticObject;

static std::string intToHex(uint64_t value) {
	std::stringstream stream;
	stream << std::hex << value;
	return stream.str();
}

// ==================== Imgui ====================
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_settings.h"
#include "external/imgui/imgui_impl_dx11.h"
#include "external/imgui/imgui_impl_win32.h"
#include "external/imgui/imgui_freetype.h"
#include "external/imgui/imgui_internal.h"
#include "external/imgui/stb_image.h"
#include "external/imgui/InputUtil.h"
#include "external/imgui/Hotkey.h"

// ==================== Render Definitions ====================
namespace render {
	extern int width, height;
	extern HWND window;

	static IDXGISwapChain* pSwapChain;
	static ID3D11Device* pDevice;
	static ID3D11DeviceContext* pDeviceContext;
	static ID3D11RenderTargetView* pRenderTargetView;
}

// ==================== Kiero ====================
#include "external/kiero/kiero.h"
#include "external/kiero/win32_impl.h"
#include "external/kiero/d3d11_impl.h"

// ==================== Utility Headers ====================
#include "utility/math.hpp"
#include "utility/memory.h"
#include "utility/offsets.h"

// ==================== Global Variables/Configs ====================
#include "globals.h"

// ==================== Decryption ====================
#include "ow/sdk/decryption/decrypt.h"

// ==================== Config ====================
#include "base/config/config.h"

// ==================== Components & Entities & SDK ====================
#include "ow/components/components.h"
#include "ow/entity.h"
#include "ow/sdk/sdk.h"	

// ==================== Updates & Features ====================
#include "base/update/update.h"
#include "base/features/features.h"
#include "base/base.h"