#include "../../../includes.h"

Value HealthComponent::MinMaxPool(std::vector<HealthPool> pools) const {
	Value result{};
	for (auto& pool : pools) {
		result.Maximum += pool.maxHealth;
		result.Current += pool.currentHealth;
	}
	return result;
}

Value HealthComponent::Health() const
{
	std::vector<HealthPool> pools;
	for (int i = 0; i < 10; i++) {
		HealthPool pool = RPM<HealthPool>(GetComponentBase() + Offsets::Health::Health + (sizeof(HealthPool) * i));
		pools.push_back(pool);
	}

	Value result = MinMaxPool(pools);
	return result;
}

Value HealthComponent::Armor() const
{
	std::vector<HealthPool> pools;
	for (int i = 0; i < 10; i++) {
		HealthPool pool = RPM<HealthPool>(GetComponentBase() + Offsets::Health::Armor + (sizeof(HealthPool) * i));
		pools.push_back(pool);
	}

	Value result = MinMaxPool(pools);
	return result;
}

Value HealthComponent::Shields() const
{
	std::vector<HealthPool> pools;
	for (int i = 0; i < 10; i++) {
		HealthPool pool = RPM<HealthPool>(GetComponentBase() + Offsets::Health::Shields + (sizeof(HealthPool) * i));
		pools.push_back(pool);
	}

	Value result = MinMaxPool(pools);
	return result;
}

Value HealthComponent::Life() const
{
	return Health() + Armor() + Shields();
}

bool HealthComponent::IsAlive() const {
	return Health().Current > 0.f;
}