#pragma once
#include "decryption/decrypt.h"

namespace RayCast {

    enum RayMask : uint64_t {
        DEFAULT = 0,
        IGNORE_SHIELD = 0x690000000000051,
        IGNORE_SHIELD_NEW = 0x69000000000003D, // not ignore shield, idk how to explain it, but filters for players and shield, 7 for shield, 15 for player.
        IGNORE_OBJECT = 0x690000000000015,
        IGNORE_PLAYER = 0x690000000000016,
        IGNORE_ALL = 0x690000000000017,
        IGNORE_UNKOWN_A = 0x690000000000018,
        IGNORE_UNKOWN_B = 0x690000000000019,
        IGNORE_UNKNOWN_B = 0x69000000000001A,
        IGNORE_UNKOWN_C = 0x690000000000029,
        IGNORE_UNKOWN_D = 0x69000000000002A,
        IGNORE_UNKOWN_E = 0x69000000000002B,
        IGNORE_UNKOWN_F = 0x69000000000002C,
        IGNORE_UNKOWN_G = 0x69000000000002D, // success on shield, fail on player
        IGNORE_UNKOWN_H = 0x6900000000000A1,
        BASIC_MASK = 0x690000000000001,
        IGNORE_UNKOWN_J = 0x690000000000002, // nvm
    };

    class RayCastIn {
    public:
        Vector3  start;        // 0x0
        char    pad_0001[0x4];    // 0xC
        Vector3  end;          // 0x10
        char    pad_0002[0x4];    // 0x1C
        char    pad_0003[0x38];    // 0x20
        uintptr_t  mask;    // 0x58
        ArrayWrapper<uint64_t>  ignore;    // 0x60
        char    pad_0004[0x208];  // 0x70
    };

    class RayCastOut {
    public:
        Vector3 location; // 0x0
    private: char pad_c[0x4]; public:
        Vector3 target; // 0x10
    private: char pad_1c[0x4]; public:
        uint64_t entity; // 0x20
    private: char pad_28[0x10]; public:
        uint16_t bone; // 0x38
    private: char pad_3a[0x6]; public:
        uint64_t hb_guid; // 0x40
    }; // Size: 0x48

    enum RaycastReturnType {
        None = 0x0,
        Entity = 0x7,
        StaticObject = 0xD, // 13
        DynamicObject = 0xF, // 15
    };

    struct RaycastResult {
        RaycastReturnType type; // 0x0
		Vector3 location; // 0x4
        Vector3 normal; // 0x16
        uint64_t entity; // 0x28
        uint64_t hb_guid; // 0x30
    }; // Size: 0x38

    class Raycaster {
    private:
        bool RaytraceCtor(RayCastIn* rtIn, uint64_t admin)
        {
            if (IsValidPtr(admin))
            {
                uint64_t addr = (uint64_t)rtIn;
                *(uint32_t*)(addr + 0x6C) = 0x80000004;
                *(uint64_t*)(addr + 0x60) = addr + 0x70;
                *(uint64_t*)(addr + 0x90) = addr + 0xA0;
                *(uint32_t*)(addr + 0x9C) = 0x80000004;
                *(uint64_t*)(addr + 0xD0) = addr + 0xE0;
                *(uint32_t*)(addr + 0xDC) = 0x80000008;
                *(uint64_t*)(addr + 0x1C0) = admin;
                *(uint8_t*)(addr + 0x19D) = 0x1;


                uint64_t vTable = RPM<uint64_t>(admin);
                if (!vTable)
                    return false;


                uint64_t result = decrypt::DecryptPointer(admin, ImageBase + Offsets::Engine::RayCastBullshit) + 0x60; // 48 8D 15 ? ? ? ? 48 8B D8 E8

                if (!result)
                    return false;

                vTable = RPM<uint64_t>(result);
                if (!vTable)
                    return false;

                uint64_t vFunc = RPM<uint64_t>(vTable + 0x8);
                if (!vFunc)
                    return false;

                *(uint64_t*)(addr + 0x1B8) = reinterpret_cast<uint64_t(_fastcall*)(uint64_t)>(vFunc)(result);

                *(uint32_t*)(addr + 0x20) = 0x3F800000;
                *(uint32_t*)(addr + 0x198) = 0x10000;
                return true;
            }
            return false;
        }

    public:
        RayCastIn input;
        std::vector<uint64_t> ignored;

        Raycaster(uint64_t admin, uint64_t mask) : input() {
            if (IsValidPtr(admin)) {
				RaytraceCtor(&input, admin);
                input.mask = mask;
            }
        }
        
        RaycastResult trace(Vector3 start, Vector3 end) {
            __try {
                for (auto ignore : ignored) {
                    if (IsValidPtr(ignore) && RPM<uint64_t>(ignore - 0x8) != -1)
                        input.ignore.add(ignore);
                }
                ignored.clear();

                input.start = start;
                input.end = end;

                auto fnRayCast = reinterpret_cast<RaycastReturnType(__fastcall*)(RayCast::RayCastIn*, RayCast::RayCastOut*, uint32_t)>(ImageBase + Offsets::Engine::RayCast);

                RayCastOut output{};
                decrypt::force_decryption(ImageBase + Offsets::Engine::RayCast);
                RaycastReturnType ret = fnRayCast(&input, &output, 0);

                RaycastResult rayResult = {
                    ret,
                    output.location,
                    output.target,
                    output.entity,
                    output.hb_guid
                };

                return rayResult;
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
				printf("Raycast exception\n");
                return { RayCast::RaycastReturnType::None, Vector3(), Vector3(), 0x0, 0x0 };
            }
        }
    };
}

namespace sdk {
    extern Matrix GetMatrix();
    extern Matrix GetProjectMatrix();
    extern uint64_t GetEntityDefinitionID(uint64_t entity);
    extern float getProjSpeed(eHero HeroID);
    extern float getDelay(uint64_t HeroID);
    extern Vector3 CalcAngle(Vector3 MyPos, Vector3 EnPos);
    extern Vector3 CalcAngle(Vector3 MyPos, Vector3 EnPos, Vector3 Angle, float Speed);
    extern Vector3 HumanizeAngle(Vector3 my_angle, Vector3 angle, Vector3 smoothed_angle, float distance);
    extern Vector3 GetClosestBone(Entity entity);
    extern uint16_t GetClosestBoneID(Entity entity);
    extern int8_t IsHitable(Entity& target_entity, uint32_t targetBone, Vector3 compensation = Vector3(0.f, 0.f, 0.f));
    extern bool RayTrace(Vector3 from, Vector3 to);
    extern RayCast::RaycastResult RayTrigger(Vector3 from, Vector3 to, RayCast::RayMask ignore_level = RayCast::IGNORE_SHIELD_NEW);
}
