#include "../../../includes.h"

constexpr int MAX_TEAM_SIZE = 5;
constexpr int KEY_CODE_START_ALLY = '1';
constexpr int KEY_CODE_START_ENEMY = '6';
constexpr int KEY_CODE_WRAP_AROUND = '9' + 1;
constexpr int KEY_CODE_ZERO = '0';

static int getKeyCode(eTeam entityTeam, eTeam localTeam, size_t& allyCount, size_t& enemyCount) {
	if (entityTeam == localTeam && allyCount < MAX_TEAM_SIZE) {
		return KEY_CODE_START_ALLY + int(allyCount++);
	}
	else if (entityTeam != localTeam && enemyCount < MAX_TEAM_SIZE) {
		int keyCode = KEY_CODE_START_ENEMY + int(enemyCount++);
		if (keyCode == KEY_CODE_WRAP_AROUND) {
			keyCode = KEY_CODE_ZERO;
		}
		return keyCode;
	}
	return 0;
}

namespace misc {
	void doAutoRecall() {
		Entity localEntity = update::GetLocalEntity();
		if (!localEntity.IsValid())
			return;

		float lifeTime = localEntity.ModelComponent.GetLifeTime();
		if (lifeTime <= 3.f)
			return;

		auto localHealth = localEntity.HealthComponent.Life();
		if (localHealth.Current <= config->RecallHealth) {
			StateVariable recall = localEntity.StateComponent.GetScriptVariable(2, SKILL_COOLTIME);
			
			if (recall.initial_value == 0.f && recall.writer_count == 0)
				localEntity.ConnectionComponent.SetInput(eInput::INPUT_SKILL2);
		}
	}

	void doAutoDash() {
		Entity localEntity = update::GetLocalEntity();
		Entity bestEntity = update::GetBestEntity();
		if (!localEntity.IsValid() || !bestEntity.IsValid())
			return;

		if (localEntity.HeroComponent.HeroID() == eHero::HERO_GENJI)
		{
			float distance = bestEntity.ModelComponent.RootPos().Distance(localEntity.ModelComponent.RootPos());

			if (distance <= 15.f && bestEntity.HealthComponent.Life().Current <= config->GenjiTargetHealth)
			{
				if (sdk::IsHitable(bestEntity, false))
					localEntity.ConnectionComponent.SetInput(eInput::INPUT_SKILL1);
			}
		}
	}

	void doNutCracker() {
		Entity localEntity = update::GetLocalEntity();
		Entity bestEntity = update::GetBestEntity();
		if (!localEntity.IsValid() || !bestEntity.IsValid())
			return;

		float distance = bestEntity.ModelComponent.RootPos().Distance(localEntity.ModelComponent.RootPos());
		if (distance <= 3.f)
		{
			if (sdk::IsHitable(bestEntity, false))
				localEntity.ConnectionComponent.SetInput(eInput::INPUT_MELEE, 10);
		}
	}

	void doAutoMelee() {
		Entity localEntity = update::GetLocalEntity();
		Entity bestEntity = update::GetBestEntity();
		if (!localEntity.IsValid() || !bestEntity.IsValid())
			return;

		auto genjiBlade = localEntity.StateComponent.GetScriptVariable(6, SKILL_REMAINTIME);
		if (genjiBlade.watchers || genjiBlade.current_value > 0.f)
			return;

		float distance = bestEntity.ModelComponent.RootPos().Distance(localEntity.ModelComponent.RootPos());
		Vector3 camera = sdk::GetProjectMatrix().GetCameraVec();

		if (distance <= 3.f && bestEntity.HealthComponent.Life().Current <= 30.f)
		{
			if (sdk::IsHitable(bestEntity, false))
				localEntity.ConnectionComponent.SetInput(eInput::INPUT_MELEE, 10);
		}
	}

	void doAutoDiscord() {
		Entity localEntity = update::GetLocalEntity();
		Entity bestEntity = update::GetBestEntity();
		if (!localEntity.IsValid() || !bestEntity.IsValid())
			return;

		if (sdk::IsHitable(bestEntity, false))
			localEntity.ConnectionComponent.SetInput(eInput::INPUT_SKILL2);
	}

	void doSojournAutoRailgun() {
		Entity localEntity = update::GetLocalEntity();
		Entity bestEntity = update::GetBestEntity();

		if (!localEntity.IsValid() || !bestEntity.IsValid())
			return;

		float distance = bestEntity.ModelComponent.RootPos().Distance(localEntity.ModelComponent.RootPos());
		float charge = localEntity.StateComponent.GetScriptVariable(0xb, eScriptVariableIndex::SKILL_SOJOURN_CHARGE).current_value;
		float base_damage = 100.f, minimum_damage = 75.f, falloff_damage = base_damage;

		if (distance > 40.f && distance < 60.f) {
			const float max_distance = 60.f, min_distance = 40.f;
			float normalized_distance = (distance - min_distance) / (max_distance - min_distance);
			falloff_damage = normalized_distance * minimum_damage + (1.00f - normalized_distance) * base_damage;
		}
		else if (distance > 60.f)
			falloff_damage = minimum_damage;

		float final_damage = charge * falloff_damage;
		uint8_t result = sdk::IsHitable(bestEntity, false);

		if (!result || config->SojAutoRightClickHeadOnly && result < 2)
			return;

		if (result == 2)
			final_damage *= 1.5f;

		if (charge > 0.f && bestEntity.HealthComponent.Life().Current < final_damage)
			localEntity.ConnectionComponent.SetInput(eInput::INPUT_RMOUSE, 10);
	}
}