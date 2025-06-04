#pragma once

enum eComponentType
{
    TYPE_MODEL = 0x4,
    TYPE_AIPOINTFINDER = 0x21,
    TYPE_BATTLETAG = 0x25,
    TYPE_ROTATE = 0x2F,
    TYPE_CONTROLLER = 0x34,
    TYPE_VISIBILITY = 0x35,
    TYPE_STATE = 0x37,
    TYPE_HEALTH = 0x3B,
    TYPE_CONNECTION = 0x43,
    TYPE_HEROID = 0x54,
    TYPE_OUTLINE = 0x5B,
};

template <uint8_t N>
class Component
{
private:
    uint64_t componentBase = 0;
    uint64_t componentParent = 0;

public:
    Component(uint64_t ComponentParent) : componentBase(decrypt::getEntityComponents(ComponentParent)[N]), componentParent(ComponentParent) {}

    uint64_t* vtable_pointer; // 0x0
    // Metadata: hex
    uint64_t encrypted_parent_entity; // 0x8
    uint8_t component_id; // 0x10
private: char pad_11[0x3]; public:
    uint32_t component_active; // 0x14

    bool IsValid() const
    {
        if (!IsValidPtr(componentBase))
            return false;

        bool componentTypeCorrect = RPM<uint8_t>(componentBase + Offsets::Component::Type) == N;

        return componentTypeCorrect; // check if component type matches and if component is active
    }

    uint64_t GetComponentBase() const
    {
        if (IsValid())
            return componentBase;
        return 0;
    }

    void SetComponentBase(uint64_t value)
    {
        componentBase = value;
    }

    void Update(std::unordered_map<uint8_t, uint64_t> entityComponents)
    {
        if (!IsValid())
            componentBase = entityComponents[N];
    }
};