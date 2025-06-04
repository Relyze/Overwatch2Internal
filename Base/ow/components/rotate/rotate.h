#pragma once

class RotateComponent : public Component<TYPE_ROTATE>
{
public:
	RotateComponent(uint64_t ComponentParent = 0) : Component<TYPE_ROTATE>(ComponentParent) {}

    Vector3 Rotation() const;
    /*
    Vector3 Velocity() const
    {
        uint64_t rotationStruct = RPM<uint64_t>(GetComponentBase() + Offsets::Rotate::RotationStruct);
        printf("rotationStruct: %llx\n", rotationStruct);
        return RPM<Vector3>(rotationStruct + Offsets::Rotate::Velocity);
    }
    */
};