#include "../../../includes.h"

bool ConnectionComponent::GetReload() const
{
    if (!IsValid())
        return false;

    uint64_t base = GetComponentBase();
    if (!IsValidPtr(base))
        return false;

    // Check the reload statuses
    if (RPM<uint8_t>(base + Offsets::Connection::Reload) == 0x4 ||
        RPM<uint8_t>(base + Offsets::Connection::Reload) == 0x84 ||
        RPM<uint8_t>(base + Offsets::Connection::WinReload) == 0x4)
        // 0x288 == default / 0x291 == winston 
        // 0x4 == default / 0x84 ana & ashe & etc 
        return false;

    return true;
}

void ConnectionComponent::SetInput(uint32_t Input) const
{
    if (!IsValid())
        return;

    uint64_t base = GetComponentBase();
    if (!IsValidPtr(base))
        return;

    *(uint32_t*)(base + Offsets::Connection::KeycodeInput) |= Input; // keycode = keycode | input
}

void ConnectionComponent::SetInput(uint32_t Input, float Time) const
{
    if (!IsValid())
        return;

    uint64_t base = GetComponentBase();
    if (!IsValidPtr(base))
        return;

    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < Time)
    {
        if (Input & eInput::INPUT_LMOUSE_UNHOLD)
            *(uint32_t*)(base + Offsets::Connection::KeycodeInput) &= ~eInput::INPUT_LMOUSE;
        else
            *(uint32_t*)(base + Offsets::Connection::KeycodeInput) |= Input;
    }
}

void ConnectionComponent::SetAngle(Vector3 Angle) const
{
    if (!IsValid())
        return;

    uint64_t base = GetComponentBase();
    if (!IsValidPtr(base))
        return;

    WPM<Vector3>(base + Offsets::Connection::ViewAngle, Angle);
}

Vector3 ConnectionComponent::Angle() const
{
    if (!IsValid())
        return Vector3();

    uint64_t base = GetComponentBase();
    if (!IsValidPtr(base))
        return Vector3();

    return RPM<Vector3>(base + Offsets::Connection::ViewAngle);
}
