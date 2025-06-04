#pragma once

#include "imgui.h"

class KeyBind;

namespace ImGuiCustom
{
    void hotkey(const char* label, KeyBind& key, bool& flag) noexcept;
}