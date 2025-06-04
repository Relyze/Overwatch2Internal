#include "../../../includes.h"

Vector3 RotateComponent::Rotation() const
{
    uint64_t rotationStruct = RPM<uint64_t>(GetComponentBase() + Offsets::Rotate::RotationStruct);
    Vector3 rotation = RPM<Vector3>(rotationStruct + Offsets::Rotate::Yaw);
    return rotation;
}