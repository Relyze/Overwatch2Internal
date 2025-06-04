#pragma once
#include "includes.h"

// Globals
namespace globals {
    extern std::atomic_bool g_Running;
	extern std::atomic_bool g_MenuState;
	extern std::atomic_bool g_Debug;
	inline std::atomic_bool g_RemoveOutlines = false;
	extern std::atomic_bool g_SaveConfig;

    extern std::string g_HeroName;
    extern std::string g_PrevHeroName;
	extern unsigned int Width, Height;
    inline std::unordered_map<int, bool> g_keyState;

	extern uint64_t GameAdmin;
	extern uint64_t ViewMatrix;
	extern uint64_t ViewMatrix2;
	extern uint64_t CameraManager;
}