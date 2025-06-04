#pragma once

struct Value {
	float Current;
	float Maximum;

	Value operator+(const Value& other) const {
		Value result;
		result.Maximum = this->Maximum + other.Maximum;
		result.Current = this->Current + other.Current;
		return result;
	}
};

struct HealthPool {
	uint32_t poolID;
	float maxHealth;
	float currentHealth;
	uint32_t unknown1;
	uint32_t unknown2;
};

class HealthComponent : public Component<TYPE_HEALTH>
{
public:
	HealthComponent(uint64_t ComponentParent = 0) : Component<TYPE_HEALTH>(ComponentParent) {}

	Value MinMaxPool(std::vector<HealthPool> pools) const;
	Value Health() const;
	Value Armor() const;
	Value Shields() const;
	Value Life() const;
	bool IsAlive() const;
};
