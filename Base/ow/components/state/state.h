#pragma once

enum eScriptVariableIndex
{
    SKILL_1 = 0x28E3, //10467
    SKILL_2 = 0x28E9, //10473
    SKILL_L = 0x2B1, //689
    SKILL_R = 0x2B2, //690
    SKILL_ULT = 0x156, //342
    SKILL_MELEE = 0x7BE, //1982
    SKILL_ULTIMATE_CHARGE = 0x1E32,
    SKILL_COOLTIME = 0x3D,
    SKILL_CHARGE = 0xF3,
    SKILL_SOJOURN_CHARGE = 0xB0,
    SKILL_REMAINTIME = 0xD7,
    SKILL_DELAY = 0xAD,
};

struct ValueKey {
    uint64_t value; // 0x0
    uint16_t key; // 0x8
}; // size: 0xA

struct KeyValue {
    uint16_t key; // 0x0
    uint64_t value; // 0x2
}; // size: 0xA

struct VarBagEntry {
    KeyValue* arr; // 0x0
    uint32_t index; // 0x8
    uint64_t unk1; // 0xC
}; // Size: 0x10

struct StateVariable {
    uint64_t* ptr_self; // 0x0
    uint64_t* readers; // 0x8
    uint32_t reader_count; // 0x10
private: char pad_14[0x2c]; public:
    uint64_t* writers; // 0x40
    uint32_t writer_count; // 0x48
private: char pad_4c[0x4]; public:
    float initial_value; // 0x50
private: char pad_54[0xc]; public:
    float current_value; // 0x60
private: char pad_64[0x2c]; public:
    uint64_t owner; // 0x90
private: char pad_98[0x14]; public:
    uint32_t id; // 0xac
private: char pad_b0[0x68]; public:
    uint32_t watchers; // 0x118
}; // Size: 0x11c

class StateComponent : public Component<TYPE_STATE>
{
public:
    StateComponent(uint64_t ComponentParent = 0) : Component<TYPE_STATE>(ComponentParent) {}

    // container
    uint64_t* GetContainer() const
    {
        uint64_t* container = (uint64_t*)(GetComponentBase() + Offsets::StateScript::ScriptContainer);
        return container;
    }

    StateVariable GetScriptVariable(uint16_t script_index, uint16_t variable) const
    {
        __try {
            if (!variable || !IsValid())
                return StateVariable();

            uint64_t globalVarBag = GetComponentBase() + Offsets::StateScript::ScriptContainer + 0x4A0;

			uint16_t scriptData[4] = { script_index, 0, variable, 0 };
            auto GetStateVariable = reinterpret_cast<uint64_t(__fastcall*)(uint64_t, uint16_t*)>(ImageBase + Offsets::StateScript::GetVarBag);
            if (!GetStateVariable)
                return StateVariable();

            decrypt::force_decryption(ImageBase + Offsets::StateScript::GetVarBag);
            uint64_t stateVariable = GetStateVariable(globalVarBag, scriptData);
            if (IsValidPtr(stateVariable))
            {
                StateVariable varResult = RPM<StateVariable>(stateVariable);

                return varResult;
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return StateVariable();
        }
        return StateVariable();
    }
};