#include "../../../includes.h"

inline uint64_t DecryptOutline(uint64_t a1)
{
	return RPM<uint32_t>(a1);
}

void OutlineComponent::SetOutline(uint32_t Flag, ImColor Outline) {
	if (!IsValid())
		return;

	OutlineMask* outlineMask = GetOutlineStruct();
	OutlineColor* outlineColor = GetOutlineColorStruct();
	OutlineOpacity* outlineOpacity = GetOutlineOpacityStruct();
	OutlineFade* outlineFade = GetOutlineFadeStruct();

	if (outlineMask != nullptr && outlineColor != nullptr && outlineOpacity != nullptr && outlineFade != nullptr)
	{
		outlineMask->firstMask = Flag;
		outlineMask->secondMask = Flag; // not necessary?

		if (original_color == 0x0) {
			original_color = outlineColor->outlineColor;
			config->OriginalEnemyColor = ImGui::ColorConvertU32ToFloat4(original_color);
		}

		outlineColor->outlineColor = ImGui::GetColorU32(Outline.Value);
		outlineOpacity->outlineOpacity = Outline.Value.w * 255.f;
		outlineFade->outlineFade = ImVec2(255, 255);
	}
}

void OutlineComponent::GetOutlineInfo(uint32_t& Flag, ImColor& Outline) {
	if (!IsValid())
		return;

	OutlineMask* outlineMask = GetOutlineStruct();
	OutlineColor* outlineColor = GetOutlineColorStruct();

	if (outlineMask != nullptr && outlineColor != nullptr)
	{
		Flag = uint32_t(outlineMask->firstMask);
		Outline = (ImColor)(outlineColor->outlineColor);
	}
}