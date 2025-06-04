#include "../../includes.h"
#include <direct.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>


Config::Config() 
{
	std::string appdata = getenv(("appdata"));
	appdata += ("\\spirit");

	mkdir(appdata.c_str());
}

json ImVec4ToJson(const ImVec4& vec) {
	return json::array({ vec.x, vec.y, vec.z, vec.w });
}

// Function to convert json::array to ImVec4
ImVec4 JsonToImVec4(const json& arr) {
	return ImVec4(arr[0], arr[1], arr[2], arr[3]);
}

namespace fs = std::filesystem;
void Config::Save(std::string hero)
{
	json configJson;

	if (hero.empty() || hero == "None")
		return;

	//std::string appdata = getenv(("appdata"));
	//appdata += ("\\spirit\\") + std::string(hero) + ".json";

	//configJson[enc("Aimbot")] = Aimbot;
	//configJson[enc("aimbotSpeed")] = aimbotSpeed;
	//configJson[enc("aimbotFOV")] = aimbotFOV;
	//configJson[enc("aimbotMinSpeed")] = aimbotMinSpeed;
	//configJson[enc("aimbotTarget")] = aimbotTarget;
	//configJson[enc("ignoreShield")] = ignoreShield;
	//configJson[enc("Trigger")] = Trigger;
	//configJson[enc("FOV")] = FOV;
	//configJson[enc("BattleTagName")] = BattleTagName;
	//configJson[enc("distanceHeroName")] = distanceHeroName;
	//configJson[enc("healthBar")] = healthBar;
	//configJson[enc("Skeleton")] = Skeleton;
	//configJson[enc("Outline")] = Outline;
	//configJson[enc("AutoShoot")] = AutoShoot;
	//configJson[enc("PredEsp")] = PredEsp;
	//configJson[enc("WidowEsp")] = WidowEsp;

	//configJson[enc("Snaplines")] = Snaplines;
	//configJson[enc("snaplineType")] = snaplineType;

	//configJson[enc("Box")] = Box;
	//configJson[enc("BoxType")] = boxType;

	//// Box Colors
	//json boxColorJson = ImVec4ToJson(BoxColor);
	//configJson[enc("BoxColor")] = boxColorJson;

	//json BoxColorInvisibleJson = ImVec4ToJson(BoxColorInvisible);
	//configJson[enc("BoxColorInvisible")] = BoxColorInvisibleJson;

	//json BoxTargetEnemyColorJson = ImVec4ToJson(BoxTargetEnemyColor);
	//configJson[enc("BoxTargetEnemyColor")] = BoxTargetEnemyColorJson;

	//json BoxUltEnemyColorJson = ImVec4ToJson(BoxUltEnemyColor);
	//configJson[enc("BoxUltEnemyColor")] = BoxUltEnemyColorJson;

	//// Snapline Colors
	//json snaplineColorJson = ImVec4ToJson(snaplineColor);
	//configJson[enc("snaplineColor")] = snaplineColorJson;

	//json snaplineColorInvisibleJson = ImVec4ToJson(snaplineColorInvisible);
	//configJson[enc("snaplineColorInvisible")] = snaplineColorInvisibleJson;

	//json snaplineTargetEnemyColorJson = ImVec4ToJson(snaplineTargetEnemyColor);
	//configJson[enc("snaplineTargetEnemyColor")] = snaplineTargetEnemyColorJson;

	//json snaplineUltEnemyColorJson = ImVec4ToJson(snaplineUltEnemyColor);
	//configJson[enc("snaplineUltEnemyColor")] = snaplineUltEnemyColorJson;

	//// Outline Colors
	//json InvisibleEnemyColorJson = ImVec4ToJson(InvisibleEnemyColor);
	//configJson[enc("InvisibleEnemyColor")] = InvisibleEnemyColorJson;

	//json TargetEnemyColorJson = ImVec4ToJson(TargetEnemyColor);
	//configJson[enc("TargetEnemyColor")] = TargetEnemyColorJson;

	//json UltEnemyColorJson = ImVec4ToJson(UltEnemyColor);
	//configJson[enc("UltEnemyColor")] = UltEnemyColorJson;

	//configJson[enc("Head")] = Head;
	//configJson[enc("Left")] = Left;
	//configJson[enc("Prediction")] = Prediction;
	//configJson[enc("Gravity")] = Gravity;
	//configJson[enc("Speed")] = Speed;
	//configJson[enc("DrawFOV")] = DrawFOV;
	//configJson[enc("AutoMelee")] = AutoMelee;
	//configJson[enc("Melee_Speed")] = Melee_Speed;

	//configJson[enc("GenjiAutoDash")] = GenjiAutoDash;
	//configJson[enc("GenjiTargetHealth")] = GenjiTargetHealth;
	//configJson[enc("AutoRecall")] = AutoRecall;
	//configJson[enc("RecallHealth")] = RecallHealth;
	//configJson[enc("ZenAutoDiscord")] = ZenAutoDiscord;
	//configJson[enc("SojAutoRightClick")] = SojAutoRightClick;
	//configJson[enc("SojAutoRightClickHeadOnly")] = SojAutoRightClickHeadOnly;

	//configJson[enc("boneKeyId")] = boneKeyId;
	//configJson[enc("aimbotKeyID")] = aimbotKeyID.toInt();
	//configJson[enc("triggerbotKeyID")] = triggerbotKeyID.toInt();

	//std::string filename = appdata;
	//std::ofstream configFile(filename);

	//if (!configFile.is_open())
	//	return;

	//configFile << std::setw(4) << configJson << std::endl;
	//configFile.close();
}

void Config::Load(std::string hero)
{
	if (hero.empty() || hero == "None")
		return;

	//std::string filename = getenv(("appdata"));
	//filename += ("\\spirit\\") + std::string(hero) + ".json";
	//std::ifstream configFile(filename);
	//if (!configFile.is_open())
	//	return;

	//json configJson;
	//try {
	//	configFile >> configJson; // Parse the JSON file
	//}
	//catch (json::parse_error& e) {
	//	return;
	//}

	//Aimbot = configJson.value(enc("Aimbot"), Aimbot);
	//aimbotSpeed = configJson.value(enc("aimbotSpeed"), aimbotSpeed);
	//aimbotFOV = configJson.value(enc("aimbotFOV"), aimbotFOV);
	//aimbotMinSpeed = configJson.value(enc("aimbotMinSpeed"), aimbotMinSpeed);
	//aimbotTarget = configJson.value(enc("aimbotTarget"), aimbotTarget);
	//Trigger = configJson.value(enc("Trigger"), Trigger);
	//FOV = configJson.value(enc("FOV"), FOV);
	//BattleTagName = configJson.value(enc("BattleTagName"), BattleTagName);
	//distanceHeroName = configJson.value(enc("distanceHeroName"), distanceHeroName);
	//healthBar = configJson.value(enc("healthBar"), healthBar);
	//Box = configJson.value(enc("Box"), Box);
	//boxType = configJson.value(enc("BoxType"), boxType);

	//json boxColorJson = configJson.value(enc("BoxColor"), ImVec4ToJson(BoxColor));
	//if (boxColorJson.is_array() && boxColorJson.size() == 4)
	//	BoxColor = JsonToImVec4(boxColorJson);

	//json BoxColorInvisibleJson = configJson.value(enc("BoxColorInvisible"), ImVec4ToJson(BoxColorInvisible));
	//if (BoxColorInvisibleJson.is_array() && BoxColorInvisibleJson.size() == 4)
	//	BoxColorInvisible = JsonToImVec4(BoxColorInvisibleJson);

	//json BoxTargetEnemyColorJson = configJson.value(enc("BoxTargetEnemyColor"), ImVec4ToJson(BoxTargetEnemyColor));
	//if (BoxTargetEnemyColorJson.is_array() && BoxTargetEnemyColorJson.size() == 4)
	//	BoxTargetEnemyColor = JsonToImVec4(BoxTargetEnemyColorJson);

	//json BoxUltEnemyColorJson = configJson.value(enc("BoxUltEnemyColor"), ImVec4ToJson(BoxUltEnemyColor));
	//if (BoxUltEnemyColorJson.is_array() && BoxUltEnemyColorJson.size() == 4)
	//	BoxUltEnemyColor = JsonToImVec4(BoxUltEnemyColorJson);

	//json snaplineColorJson = configJson.value(enc("snaplineColor"), ImVec4ToJson(snaplineColor));
	//if (snaplineColorJson.is_array() && snaplineColorJson.size() == 4)
	//	snaplineColor = JsonToImVec4(snaplineColorJson);

	//json snaplineColorInvisibleJson = configJson.value(enc("snaplineColorInvisible"), ImVec4ToJson(snaplineColorInvisible));
	//if (snaplineColorInvisibleJson.is_array() && snaplineColorInvisibleJson.size() == 4)
	//	snaplineColorInvisible = JsonToImVec4(snaplineColorInvisibleJson);

	//json snaplineTargetEnemyColorJson = configJson.value(enc("snaplineTargetEnemyColor"), ImVec4ToJson(snaplineTargetEnemyColor));
	//if (snaplineTargetEnemyColorJson.is_array() && snaplineTargetEnemyColorJson.size() == 4)
	//	snaplineTargetEnemyColor = JsonToImVec4(snaplineTargetEnemyColorJson);

	//json snaplineUltEnemyColorJson = configJson.value(enc("snaplineUltEnemyColor"), ImVec4ToJson(snaplineUltEnemyColor));
	//if (snaplineUltEnemyColorJson.is_array() && snaplineUltEnemyColorJson.size() == 4)
	//	snaplineUltEnemyColor = JsonToImVec4(snaplineUltEnemyColorJson);

	//json InvisibleEnemyColorJson = configJson.value(enc("InvisibleEnemyColor"), ImVec4ToJson(InvisibleEnemyColor));
	//if (InvisibleEnemyColorJson.is_array() && InvisibleEnemyColorJson.size() == 4)
	//	InvisibleEnemyColor = JsonToImVec4(InvisibleEnemyColorJson);

	//json TargetEnemyColorJson = configJson.value(enc("TargetEnemyColor"), ImVec4ToJson(TargetEnemyColor));
	//if (TargetEnemyColorJson.is_array() && TargetEnemyColorJson.size() == 4)
	//	TargetEnemyColor = JsonToImVec4(TargetEnemyColorJson);

	//json UltEnemyColorJson = configJson.value(enc("UltEnemyColor"), ImVec4ToJson(UltEnemyColor));
	//if (UltEnemyColorJson.is_array() && UltEnemyColorJson.size() == 4)
	//	UltEnemyColor = JsonToImVec4(UltEnemyColorJson);

	//Skeleton = configJson.value(enc("Skeleton"), Skeleton);
	//Outline = configJson.value(enc("Outline"), Outline);
	//AutoShoot = configJson.value(enc("AutoShoot"), AutoShoot);
	//PredEsp = configJson.value(enc("PredEsp"), PredEsp);
	//WidowEsp = configJson.value(enc("WidowEsp"), WidowEsp);

	//Snaplines = configJson.value(enc("Snaplines"), Snaplines);
	//snaplineType = configJson.value(enc("snaplineType"), snaplineType);

	//Head = configJson.value(enc("Head"), Head);
	//Left = configJson.value(enc("Left"), Left);
	//Prediction = configJson.value(enc("Prediction"), Prediction);
	//Gravity = configJson.value(enc("Gravity"), Gravity);
	//Speed = configJson.value(enc("Speed"), Speed);
	//DrawFOV = configJson.value(enc("DrawFOV"), DrawFOV);
	//AutoMelee = configJson.value(enc("AutoMelee"), AutoMelee);
	//Melee_Speed = configJson.value(enc("Melee_Speed"), Melee_Speed);

	//GenjiAutoDash = configJson.value(enc("GenjiAutoDash"), GenjiAutoDash);
	//GenjiTargetHealth = configJson.value(enc("GenjiTargetHealth"), GenjiTargetHealth);
	//AutoRecall = configJson.value(enc("AutoRecall"), AutoRecall);
	//RecallHealth = configJson.value(enc("RecallHealth"), RecallHealth);
	//ZenAutoDiscord = configJson.value(enc("ZenAutoDiscord"), ZenAutoDiscord);
	//SojAutoRightClick = configJson.value(enc("SojAutoRightClick"), SojAutoRightClick);
	//SojAutoRightClickHeadOnly = configJson.value(enc("SojAutoRightClickHeadOnly"), SojAutoRightClickHeadOnly);

	//boneKeyId = configJson.value(enc("boneKeyId"), boneKeyId);
	//aimbotKeyID.setToKey(configJson.value(enc("aimbotKeyID"), 0));
	//triggerbotKeyID.setToKey(configJson.value(enc("triggerbotKeyID"), 0));
}