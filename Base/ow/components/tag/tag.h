#pragma once

class TagComponent : public Component<TYPE_BATTLETAG>
{
public:
	TagComponent(uint64_t ComponentParent = 0) : Component<TYPE_BATTLETAG>(ComponentParent) {}

	uint32_t TagSize() const
	{
		if (IsValid())
			return RPM<uint32_t>(GetComponentBase() + Offsets::Tag::Length);
		return 0;
	}

	std::string BattleTag() const
	{
		if (IsValid())
		{
			uint64_t tagOffset = RPM<uint64_t>(GetComponentBase() + Offsets::Tag::Tag);
			uint32_t tagSize = RPM<uint32_t>(tagOffset + Offsets::Tag::Length);
			uint8_t tagutf8 = RPM<uint8_t>(tagOffset + Offsets::Tag::UTF8);
			std::string playerName;
			if (tagutf8 <= 0x10)
				playerName = AReadStringFromMemory(tagOffset, tagSize);
			else 
				playerName = AReadStringFromMemory(RPM<uint64_t>(tagOffset), tagutf8);
			return playerName;
		}
		return "AI";
	}
};