#include "../../../includes.h"

uint32_t ControllerComponent::getHeroID() const {
    uintptr_t base = GetComponentBase();
    if (IsValidPtr(base)) {
        uintptr_t heroEntityAddress = base + Offsets::Controller::heroEntity;
        if (IsValidPtr(heroEntityAddress)) {
            return RPM<uint32_t>(heroEntityAddress);
        }
    }
}