#include <sstream>
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "Hotkey.h"
#include "InputUtil.h"
#include "imgui_settings.h"

void ImGuiCustom::hotkey(const char* label, KeyBind& key, bool& flag) noexcept
{
    const auto id = ImGui::GetID(label);
    ImGui::PushID(label);

    ImGuiWindow* window = ImGui::GetCurrentWindow();

    const float w = ImGui::CalcItemWidth();
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos + ImVec2(45, 0), window->DC.CursorPos + ImVec2(w + 45, label_size.y - 5));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x, 0.0f));

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 45);
    if (ImGui::GetActiveID() == id || flag) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
        ImGui::Button("...", ImVec2(ImGui::CalcItemWidth(), 30));
        ImGui::PopStyleColor();
        ImGui::GetCurrentContext()->ActiveIdAllowOverlap = true;
        if ((!ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0]) || key.setToPressedKey())
        {
            ImGui::ClearActiveID();
            flag = false;
        }
    }
    else if (ImGui::Button(key.toString(), ImVec2(ImGui::CalcItemWidth(), 30))) {
        ImGui::SetActiveID(id, window);
        flag = true;
    }
    ImGui::PopID();

    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(c::text::text));
    ImGui::RenderText(ImVec2(frame_bb.Max.x + 15, frame_bb.Min.y + 5), label);
    ImGui::PopStyleColor();

}