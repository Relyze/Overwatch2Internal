#define STB_IMAGE_IMPLEMENTATION
#include "includes.h"
#include "external/minhook/include/MinHook.h"
static FILE* file = NULL;
HANDLE	m_hTimerQueue;
HANDLE	m_hTimerQueueTimer;

// Initialize our global variables
namespace globals {
	std::atomic_bool g_Running = true;
	std::atomic_bool g_MenuState = true;
	std::atomic_bool g_Debug = true;
	std::string g_HeroName, g_PrevHeroName = "";
	std::atomic_bool g_SaveConfig = false;

	uint64_t GameAdmin = 0;
	uint64_t ViewMatrix = 0;
	uint64_t ViewMatrix2 = 0;
	uint64_t CameraManager = 0;
}

static int mainThread(HMODULE hModule)
{
	if (globals::g_Running)
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			impl::d3d11::init();

			base::initializeBase();

			std::string previous_hero;
			bool first_hero = true;
			while (true)
			{
				if (update::isIngame) {
					if (globals::g_HeroName == "None")
						globals::g_SaveConfig = true;

					if (globals::g_SaveConfig && !globals::g_HeroName.empty() && globals::g_HeroName != "None") {
						if (previous_hero.empty() || previous_hero == "None")
							previous_hero = globals::g_HeroName;

						if (!first_hero) {
							if (previous_hero != globals::g_HeroName || (globals::g_SaveConfig && !globals::g_MenuState)) {
								config->Save(previous_hero);
								globals::g_SaveConfig = false;
							}
						}

						previous_hero = globals::g_HeroName;
						config->Load(globals::g_HeroName);

						if (first_hero)
							first_hero = false;
					}
				}

				if (GetAsyncKeyState(VK_END))
					globals::g_Running = false;
				if (GetAsyncKeyState(VK_INSERT) & 0x1) {
					globals::g_MenuState = !globals::g_MenuState;
					if (!globals::g_MenuState && update::isIngame)
						globals::g_SaveConfig = true;
				}
				if (!globals::g_Running) {
					globals::g_RemoveOutlines = true;
					Sleep(200);
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			
		}
	}

	// Cleanup
	kiero::shutdown();
	impl::win32::unhook(render::window);

	if (globals::g_Debug)
	{
		printf(enc("Done!").c_str());
		Sleep(500);
		fclose(file);
		FreeConsole();
	}
	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

static bool exceptionSupport = false;
static bool AddExceptionSupport(HMODULE hModule) {
	if (!hModule) {
		// Invalid module handle
		printf("Invalid module handle\n");
		return false;
	}

	// Get the DOS header
	auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		// Invalid DOS header signature
		printf("Invalid DOS header signature\n");
		return false;
	}

	// Get the NT headers
	auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS64>(reinterpret_cast<BYTE*>(hModule) + dosHeader->e_lfanew);
	if (ntHeader->Signature != IMAGE_NT_SIGNATURE) {
		// Invalid NT header signature
		printf("Invalid NT header signature\n");
		return false;
	}

	// Check if the module has an exception directory
	auto& excep = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
	if (excep.Size == 0 || excep.VirtualAddress == 0) {
		// No exception directory present
		printf("No exception directory present\n");
		return false;
	}

	// Calculate the function table and number of entries
	auto pFunctionTable = reinterpret_cast<PIMAGE_RUNTIME_FUNCTION_ENTRY>(reinterpret_cast<BYTE*>(hModule) + excep.VirtualAddress);
	DWORD numEntries = excep.Size / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY);

	// Ensure the function table is within the module's memory bounds
	if (reinterpret_cast<BYTE*>(pFunctionTable) + excep.Size > reinterpret_cast<BYTE*>(hModule) + ntHeader->OptionalHeader.SizeOfImage) {
		// Function table is out of bounds
		printf("Function table is out of bounds\n");
		return false;
	}

	// Add the function table to the runtime
	if (!RtlAddFunctionTable(pFunctionTable, numEntries, reinterpret_cast<DWORD64>(hModule))) {
		// Failed to add function table
		printf("Failed to add function table\n");
		return false;
	}

	return true;
}

static VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	HMODULE hModule = (HMODULE)lpParam;
	mainThread(hModule);
}

BOOL WINAPI DllMain(const HMODULE hModule, const DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
			exceptionSupport = AddExceptionSupport(hModule);

			if (globals::g_Debug)
			{
				AllocConsole();
				freopen_s(&file, "CONIN$", "rb", stdin);
				freopen_s(&file, "CONOUT$", "wb", stdout);
				printf(enc("Exception support: %s\n").c_str(), exceptionSupport ? "true" : "false");
			}

			if (!m_hTimerQueue && !m_hTimerQueueTimer) {
				m_hTimerQueue = CreateTimerQueue();
				if (m_hTimerQueue && !CreateTimerQueueTimer(&m_hTimerQueueTimer, m_hTimerQueue, TimerRoutine, hModule, 4000, 0, WT_EXECUTEDEFAULT)) {
					m_hTimerQueue = nullptr;
					m_hTimerQueueTimer = nullptr;
				}
			}
			break;

		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
