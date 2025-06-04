#pragma once

using json = nlohmann::json;

static int selectedConfig = 0;
class Config
{
public:
	Config();

	// Outlines
	ImVec4 OriginalEnemyColor = { 1.f, 0.f, 0.f, 1.f };
	ImVec4 InvisibleEnemyColor = { 0.773f, 0.094f, 0.086f, 1.f };
	ImVec4 TargetEnemyColor = { 1.f, 0.f, 0.f, 1.f };
	ImVec4 UltEnemyColor = { 0.849f, 0.141f, 0.267f, 1.f };

	// Boxes
	ImVec4 BoxColor = { 1.f, 1.f, 1.f, 1.f };
	ImVec4 BoxColorInvisible = { 0.815f, 0.058f, 0.203f, 1.f };
	ImVec4 BoxTargetEnemyColor = { 1.f, 0.f, 0.f, 1.f };
	ImVec4 BoxUltEnemyColor = { 0.849f, 0.141f, 0.267f, 1.f };

	// Snaplines
	ImVec4 snaplineColor = { 1.f, 1.f, 1.f, 1.f };
	ImVec4 snaplineColorInvisible = { 0.815f, 0.058f, 0.203f, 1.f };
	ImVec4 snaplineTargetEnemyColor = { 1.f, 0.f, 0.f, 1.f };
	ImVec4 snaplineUltEnemyColor = { 0.849f, 0.141f, 0.267f, 1.f };

	float FOV = 103;
	bool Trigger = false;
	bool Aimbot = true;
	bool hitboxEsp = false;
	bool BattleTagName = false;
	bool distanceHeroName = false;
	bool healthBar = false;

	float aimbotSpeed = 20.f;
	float aimbotFOV = 300.f;
	float aimbotMinSpeed = 30.f;
	int aimbotTarget = 0;
	bool ignoreShield = false;
	bool blockPrimary = true;

	int boxType = 0;
	const char* Boxes[3] = { "Corner", "Full" , "Rounded Full" };
	bool Box = false;

	bool Skeleton = false;
	bool Outline = false;
	bool AutoShoot = true;

	bool Head = false;
	bool Left = true;
	bool Prediction = false;
	bool Gravity = false;
	float Speed = 110.f;
	bool PredEsp = false;
	bool WidowEsp = false;

	const char* snaplineTypes[3] = { "Bottom", "Top", "Center" };
	bool Snaplines = false;
	int snaplineType = 0;

	bool DrawFOV = true;

	bool AutoMelee = true;
	float Melee_Speed = 50.f;

	// Tracer
	bool AutoRecall = false;
	float RecallHealth = 50.f;

	//genji
	bool GenjiAutoDash = false;
	float GenjiTargetHealth = 50.f;

	//zen
	bool ZenAutoDiscord = false;

	//Soj
	bool SojAutoRightClick = false;
	bool SojAutoRightClickHeadOnly = false; // stupid shit fuck you 

	int boneKeyId = 0;
	const char* boneKeys[5] = { "Closest", "Head", "Neck", "Body", "Nuts" };

	KeyBindToggle aimbotKeyID = KeyBindToggle();
	KeyBindToggle triggerbotKeyID = KeyBindToggle();
	void Save(std::string hero);
	void Load(std::string hero);
};

inline Config* config = new Config;