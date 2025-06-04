#pragma once

enum OutlineType : uint32_t {
	VISIBLE = 1,
	ESP = 2,
	INVISIBLE = 3,
};

struct OutlineMask {
	char pad0[0x8]; // 0x0
	uint64_t firstMask; // 0x8
	uint64_t secondMask; // 0x10
};

struct OutlineColor {
	uint64_t pad0; // 0x0
	uint64_t pad8; // 0x8
	ImU32	outlineColor; // 0x10
};

struct OutlineOpacity {
	uint64_t pad0; // 0x0
	uint64_t pad8; // 0x8
	float outlineOpacity; // 0x10
};

struct OutlineFade {
	uint64_t pad0; // 0x0
	uint64_t pad8; // 0x8
	ImVec2 outlineFade; // 0x10
};

class OutlineComponent : public Component<TYPE_OUTLINE>
{
private:
	uint32_t original_color = 0x0;
public:
	OutlineComponent(uint64_t ComponentParent = 0) : Component<TYPE_OUTLINE>(ComponentParent) {}

	void SetOutline(uint32_t Flag, ImColor Outline);
	void GetOutlineInfo(uint32_t& Flag, ImColor& Outline);

private:
	OutlineMask* GetOutlineStruct() const
	{
		OutlineMask* result = nullptr;
		uint64_t v1 = GetComponentBase() + 0x20;
		int v2 = RPM<int>(v1 + 0x68);

		if ((uint16_t)v2)
			result = (OutlineMask*)(0x20 * uint64_t(v2) + RPM<uint64_t>(v1 + 0x60) - 0x20);
		return result;
	}

	OutlineColor* GetOutlineColorStruct() const
	{
		OutlineColor* result = nullptr;
		uint64_t v1 = GetComponentBase() + 0x20;
		int v2 = RPM<int>(v1 + 0xD8);

		if ((uint16_t)v2)
			result = (OutlineColor*)(RPM<uint64_t>(v1 + 0xD0) + 0x14 * uint64_t(v2) - 0x14);

		return result;
	}

	OutlineOpacity* GetOutlineOpacityStruct() const
	{
		OutlineOpacity* result = nullptr;
		uint64_t v1 = GetComponentBase() + 0x20;
		int v2 = RPM<int>(v1 + 0x1A8);

		if ((uint16_t)v2)
			result = (OutlineOpacity*)(RPM<uint64_t>(v1 + 0x1A0) + 0x14 * uint64_t(v2) - 0x14);

		return result;
	}

	OutlineFade* GetOutlineFadeStruct() const
	{
		OutlineFade* result = nullptr;
		uint64_t v1 = (uint64_t)(GetComponentBase() + 0x20);
		int v2 = RPM<uint32_t>(v1 + 0x138);

		if ((uint16_t)v2)
			result = (OutlineFade*)(RPM<uint64_t>(v1 + 0x130) + 0x18 * uint64_t(v2) - 0x18);

		return result;
	}
};