#pragma once

enum eTeam : uint32_t
{
	TEAM_NONE = 0x1 << 0x00,
	TEAM_RED = 0x1 << 0x17,
	TEAM_BLUE = 0x1 << 0x18,
	TEAM_ATTACKER_CTF = 0x1 << 0x19,
	TEAM_DEFENDER_CTF = 0x1 << 0x1A,
	TEAM_FFA = 0x1 << 0x1B,
};

class TeamComponent : public Component<TYPE_AIPOINTFINDER>
{
public:
	// Constructor
	TeamComponent(uint64_t ComponentParent = 0) : Component<TYPE_AIPOINTFINDER>(ComponentParent) {}

	eTeam TeamID() const
	{
		uint32_t team = RPM<uint32_t>(GetComponentBase() + Offsets::Team::Team);
		return eTeam(team & 0xF800000);
	}

	uint32_t TeamMask() const 	{
		return (RPM<uint32_t>(GetComponentBase() + Offsets::Team::Team) & 0x7800000) >> 0x17;
	}

	bool isTeamFFA() const 	{
		return (TeamID() & eTeam::TEAM_FFA);
	}

	bool isEnemy(TeamComponent other) const {
		return isTeamFFA() || TeamID() != other.TeamID();
	}

	bool isAlly(TeamComponent other) const {
		return TeamID() == other.TeamID();
	}
};