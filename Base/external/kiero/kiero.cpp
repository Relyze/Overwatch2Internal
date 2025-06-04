#include "kiero.h"
#include "../../utility/offsets.h"
#include <Windows.h>
#include <assert.h>
#include <random>
#include <ctime>
#include "..\..\utility\memory.h"
#if KIERO_INCLUDE_D3D11
# include <dxgi.h>
# include <d3d11.h>
#endif

#if KIERO_USE_MINHOOK
#include "../minhook/include/MinHook.h"
#endif
#include <stdio.h>

#ifdef _UNICODE
# define KIERO_TEXT(text) L##text
#else
# define KIERO_TEXT(text) text
#endif

#define KIERO_ARRAY_SIZE(arr) ((size_t)(sizeof(arr)/sizeof(arr[0])))

static kiero::RenderType::Enum g_renderType = kiero::RenderType::None;
static uint150_t* g_methodsTable = NULL;
uint64_t obsBase = 0x0;
uint64_t discordBase = 0x0;
uint64_t obsPresent = 0x0;
uint64_t discordPresent = 0x0;

kiero::Status::Enum kiero::init(RenderType::Enum _renderType)
{
	if (g_renderType != RenderType::None)
	{
		return Status::AlreadyInitializedError;
	}

	if (_renderType != RenderType::None)
	{
		if (_renderType == RenderType::D3D11)
		{
#if KIERO_INCLUDE_D3D11
			uint64_t ImageBase = *reinterpret_cast<uint64_t*>(__readgsqword(0x60) + 0x10);
			uint64_t renderData = *(uint64_t*)(((*(uint64_t*)(ImageBase + Offsets::Engine::RenderData) + Offsets::Engine::RenderDataKey1) ^ Offsets::Engine::RenderDataKey2) + 0x20); //48 8B 0D ? ? ? ? 49 BE ? ? ? ? ? ? ? ? 49 03 CE 49 BF ? ? ? ? ? ? ? ? 49 33 CF 48 8B 01

			IDXGISwapChain* swapChain = *(IDXGISwapChain**)(renderData + 0x1B0); 

			g_methodsTable = (uint150_t*)::calloc(205, sizeof(uint150_t));
			if (!g_methodsTable) {
				// Handle allocation failure
				return Status::UnknownError;
			}

			std::memcpy(g_methodsTable, *(uint150_t**)swapChain, 18 * sizeof(uint150_t));

			ID3D11Device* device = nullptr;
			ID3D11DeviceContext* context = nullptr;

			if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device)))
			{
				device->GetImmediateContext(&context);

				if (device)
					std::memcpy(g_methodsTable + 18, *(uint150_t**)device, 43 * sizeof(uint150_t));
				if (context)
					std::memcpy(g_methodsTable + 18 + 43, *(uint150_t**)context, 144 * sizeof(uint150_t));

				if (context) context->Release();
				if (device) device->Release();
			}

#if KIERO_USE_MINHOOK
			MH_Initialize();
#endif
			g_renderType = RenderType::D3D11;

			return Status::Success;
#endif
		}
		return Status::NotSupportedError;
	}

	return Status::Success;
}

void kiero::shutdown()
{
	if (g_renderType != RenderType::None)
	{
#if KIERO_USE_MINHOOK
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
#endif

		::free(g_methodsTable);
		g_methodsTable = NULL;
		g_renderType = RenderType::None;
	}
}

uintptr_t getModuleBase(const char* moduleName, int maxAttempts = 5, int sleepTime = 200) {
	uintptr_t base = 0;
	int counter = 0;

	while (!(base = (uintptr_t)GetModuleHandleA(moduleName)) && counter++ < maxAttempts) {
		Sleep(sleepTime);
	}

	return base;
}

// Realistically, we should queue all the hooks, and then apply the queue according to minhook's official documentation.
static kiero::Status::Enum createAndEnableHook(uintptr_t target, void* _function, void** _original) {
	auto status = MH_CreateHook(reinterpret_cast<void*>(target), _function, reinterpret_cast<LPVOID*>(_original));
	if (status != MH_OK) {
		// print the error
		//printf("MH_CreateHook: %i\n", status);
		return kiero::Status::UnknownError;
	}

	status = MH_EnableHook(reinterpret_cast<void*>(target));
	if (status != MH_OK) {
		// print the error
		//printf("MH_EnableHook: %i\n", status);
		return kiero::Status::UnknownError;
	}

	return kiero::Status::Success;
}


kiero::Status::Enum kiero::bind(uint16_t _index, void** _original, void* _function)
{
	assert(_original != NULL && _function != NULL);

	if (g_renderType != RenderType::None)
	{
#if KIERO_USE_MINHOOK
		//uintptr_t obsBase = getModuleBase("graphics-hook64.dll");
		//uintptr_t discordBase = getModuleBase("DiscordHook64.dll");
		//uintptr_t nahimicOSDBase = getModuleBase("NahimicOSD.dll");

		//// Streamproofing
		//if (_index == 8) {
		//	if (obsBase) {
		//		uintptr_t obsPresent = obsBase + 0x40250;  // Obs offset // 48 8B 05 ?? ?? ?? ?? FF D0 C7 04 33 00 00 00 00 E9 0D 01 00 00 <- obs present, graphics-hook64.dll
		//		if (*reinterpret_cast<uintptr_t*>(obsPresent)) {
		//			return createAndEnableHook(*(uintptr_t*)(obsPresent), _function, _original);
		//		}
		//	}
		//	else if (discordBase) {
		//		discordBase = discordBase + 0x17850;  // Discord offset  // 48 8B 05 ?? ?? ?? ?? 48 89 D9 89 FA 41 89 F0 FF 15 ?? ?? ?? ?? 89 C6 48 89 E9 <- discord present, DiscordHook64.dll
		//		return createAndEnableHook(discordBase, _function, _original);
		//	}
		//	else if (nahimicOSDBase) {
		//		nahimicOSDBase = nahimicOSDBase + 0x177A0;  // NahimicOSD offset  // 4C 8D 05 ?? ?? ?? ?? 48 8B CF E8 ?? ?? ?? ?? 85 C0 75 4C <- nahimic present, NahimicOSD.dll
		//		return createAndEnableHook(nahimicOSDBase, _function, _original);
		//	}
		//}
		//if (_index == 13) {
		//	if (discordBase) {
		//		discordBase = discordBase + 0x179B0;  // Discord offset  // 4C 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 0F 84 00 FF FF FF <- discord resize buffers, DiscordHook64.dll
		//		return createAndEnableHook(discordBase, _function, _original);
		//	}
		//	else if (nahimicOSDBase) {
		//		nahimicOSDBase = nahimicOSDBase + 0x178C0;  // NahimicOSD offset  // 4C 8D 05 ?? ?? ?? ?? E8 ?? ?? ?? ?? 85 C0 75 26 <- nahimic resize buffers, NahimicOSD.dll
		//		return createAndEnableHook(nahimicOSDBase, _function, _original);
		//	}
		//}

		// Fallback to default method
		void* target = (void*)g_methodsTable[_index];
		
		return createAndEnableHook((uintptr_t)target, _function, _original);
#endif
	}
	return Status::NotInitializedError;  // In case KIERO_USE_MINHOOK is not defined
}

void kiero::unbind(uint16_t _index)
{
	if (g_renderType != RenderType::None)
	{
#if KIERO_USE_MINHOOK

		//uintptr_t obsBase = getModuleBase("graphics-hook64.dll");
		//uintptr_t discordBase = getModuleBase("DiscordHook64.dll");
		//uintptr_t nahimicOSDBase = getModuleBase("NahimicOSD.dll");

		//uintptr_t obsPresent = obsBase ? obsBase + 0x42E18 : 0;
		//uintptr_t discordPresent = discordBase ? discordBase + 0xE9078 : 0;

		//if (obsPresent) {
		//	MH_DisableHook((void*)obsPresent);
		//	MH_RemoveHook((void*)obsPresent);
		//}
		//else if (discordPresent) {

		//	MH_DisableHook((void*)discordPresent);
		//	MH_RemoveHook((void*)discordPresent);
		//}
		//else
		{
			MH_DisableHook((void*)g_methodsTable[_index]);
			MH_RemoveHook((void*)g_methodsTable[_index]);
		}
#endif
	}
}

kiero::RenderType::Enum kiero::getRenderType()
{
	return g_renderType;
}

uint150_t* kiero::getMethodsTable()
{
	return g_methodsTable;
}