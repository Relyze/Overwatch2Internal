#pragma once

class ControllerComponent : public Component<TYPE_CONTROLLER>
{
public:
	ControllerComponent(uint64_t ComponentParent = 0) : Component<TYPE_CONTROLLER>(ComponentParent) {}

	uint32_t getHeroID() const;
};