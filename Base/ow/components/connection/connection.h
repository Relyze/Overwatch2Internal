#pragma once

enum eInput
{
	INPUT_NULL = 0x0,
	INPUT_LMOUSE_UNHOLD = 0x1000,
	INPUT_LMOUSE = 0x1,
	INPUT_RMOUSE = 0x2,
	INPUT_RMOUSE_UNHOLD = 0x2000,
	INPUT_INTERACTION = 0x4,
	INPUT_SKILL1 = 0x8,
	INPUT_SKILL2 = 0x10,
	INPUT_ULT = 0x20,
	INPUT_JUMP = 0x40,
	INPUT_DUCK = 0x80,
	INPUT_RELOAD = 0x400,
	INPUT_MELEE = 0x800,
};

class ConnectionComponent : public Component<TYPE_CONNECTION>
{
public:
	ConnectionComponent(uint64_t ComponentParent = 0) : Component<TYPE_CONNECTION>(ComponentParent) {}

	bool GetReload() const;
	void SetInput(uint32_t Input) const;
	void SetInput(uint32_t Input, float Time) const;
	void SetAngle(Vector3 Angle) const;
	Vector3 Angle() const;
};