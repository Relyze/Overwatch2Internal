#pragma once
#include <iostream>

namespace Offsets
{
	namespace Engine {
		static constexpr auto GlobalAdmin = 0x368EF60; // 48 83 3D ? ? ? ? ? 75 ? 48 B8

		// This contains a lot of the render data, such as the view matrix cache, the swapchain vtables, some shader data, etc.
		static constexpr auto RenderData = 0x35C9350;
		static constexpr auto RenderDataKey1 = 0x6E442CAA91283F28;
		static constexpr auto RenderDataKey2 = 0xF6CEBEEB57F63757;

		static constexpr auto ViewMatrix = 0x3CC1138; // 48 8B 0D ? ? ? ? 4C 8B E3
		static constexpr auto ViewMatrixPointer = 0x7e0;
		static constexpr auto CameraManager = 0x3D1D860; // 48 8B 05 ? ? ? ? 48 8B 3C D8 48 3B FE
		static constexpr auto RayCastBullshit = 0x3758E90; // 48 8D 15 ? ? ? ? 48 8B D8 E8

		static constexpr auto RayCast = 0xC678E0; // E8 ? ? ? ? 0F 28 0D ? ? ? ? 48 8D 54 24 ? 0F 59 4C 24 + 0x2E
		static constexpr auto ArrayInsertU64 = 0x55A230; // E8 ? ? ? ? 41 B0 ? 48 8B D5 49 8B 4F
		static constexpr auto LocalPlayerUID = 0x2F0;
	}

	namespace Component {
		static constexpr auto EncryptedParent = 0x8;
		static constexpr auto Type = 0x10;
		static constexpr auto Active = 0x14;
		static constexpr auto ParentUID = 0xD4;
	}

	namespace StateScript {
		static constexpr auto GetVarBag = 0x20C00A0;
		static constexpr auto ScriptContainer = 0x110;
	}

	namespace Model {
		static constexpr auto Velocity = 0x50;
		static constexpr auto Parent = 0x238;
		static constexpr auto RootPosition = 0x3D0;
		static constexpr auto OutlineFlag = 0x42C;
		static constexpr auto OutlineMask = 0x430;
		static constexpr auto OutlineColor = 0x434;
		static constexpr auto OutlineFadeStart = 0x438;
		static constexpr auto OutlineFadeEnd = 0x43C;
		static constexpr auto EncryptedBuffer = 0x640;
		static constexpr auto LifeTime = 0x5F8; // outdated
		static constexpr auto SkeletalModel = 0x8B0;
	}

	namespace Tag {
		constexpr auto Tag = 0xE0;
		constexpr auto UTF8 = 0x14;
		constexpr auto Length = 0x10;
	}

	namespace Rotate {
		static constexpr auto RotationStruct = 0x888;
		static constexpr auto Yaw = 0x8FC;
		static constexpr auto Velocity = 0x814;
	}

	namespace HeroIdentifier {
		static constexpr auto HeroID = 0xD0;
		static constexpr auto SkinID = 0xE8;
	}

	namespace Team {
		static constexpr auto Team = 0x58;
	}

	namespace Health {
		static constexpr auto Health = 0xD8;
		static constexpr auto OverHealth = 0xF0;
		static constexpr auto Armor = 0x218;
		static constexpr auto Shields = 0x358;
		static constexpr auto IsInvulnerable = 0x4A8;
		static constexpr auto IsImmortal = 0x4A9;
	}

	namespace Outline {
		static constexpr auto outline_base = 0x20;
		static constexpr auto outlineType = 0x80;
		static constexpr auto outlineColor = 0x110;
	}

	namespace Controller {
		static constexpr auto heroEntity = 0xD0;
	}

	namespace Connection {
		static constexpr auto Reload = 0x297;
		static constexpr auto WinReload = 0x2A1;
		static constexpr auto HorizontalRecoil = 0x1608;
		static constexpr auto NoRecoil = 0x160C;
		static constexpr auto HorizontalMovement = 0x123F;
		static constexpr auto VerticalMovement = 0x1240;
		static constexpr auto KeycodeInput = 0x115C; 
		static constexpr auto ViewAngle = 0x11A0;
	}

	namespace Entity {
		static constexpr auto unalignedSpecifier = 0x30;
		static constexpr auto componentContainer = 0x80;
		static constexpr auto componentCount = 0x88;
		static constexpr auto entityID = 0x138;
	}
}
