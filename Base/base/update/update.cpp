#include "../../includes.h"
#include <DirectXMath.h>
#include <limits>

enum AIMTEAMFILTER
{
	OnlyEnemy,
	OnlyAlly,
	Both
};

static bool TargetFilter(const Entity& entity, const Entity& localEntity, int& teamFilter)
{
	const eHero localHero = localEntity.HeroComponent.HeroID();
	const bool isEnemy = localEntity.TeamComponent.isEnemy(entity.TeamComponent);
	const bool isTeam = localEntity.TeamComponent.isAlly(entity.TeamComponent);
	const bool isFFA = localEntity.TeamComponent.isTeamFFA();

	const bool enemyOnly = teamFilter == AIMTEAMFILTER::OnlyEnemy && isEnemy;
	const bool allyOnly = teamFilter == AIMTEAMFILTER::OnlyAlly && isTeam;

	if (localHero == HERO_LIFEWEAVER)
	{
		const auto variable = localEntity.StateComponent.GetScriptVariable(0xC, 0xE7);
		teamFilter = (variable.writer_count) ? AIMTEAMFILTER::OnlyEnemy : AIMTEAMFILTER::OnlyAlly;
	}

	const Value life = entity.HealthComponent.Life();
	if (life.Current <= 0 || !(enemyOnly || allyOnly || teamFilter == AIMTEAMFILTER::Both))
		return false;

	if (isTeam && (life.Current / life.Maximum) >= 1.0f && !isFFA)
		return false;

	return true;
}

static void CalculateBestEntity()
{
	const Entity localEntity = update::GetLocalEntity();
	if (!localEntity.IsValid())
		return;

	float aimbotFOV = config->aimbotFOV;
	const std::vector<Entity> entities = update::GetEntities();

	Matrix vm = sdk::GetMatrix();
	const Vector2 screenCenter(render::width * 0.5f, render::height * 0.5f);

	Entity bestEntity = Entity(0, 0);
	float bestEntityScore = std::numeric_limits<float>::max();

	Vector3 localPosition = localEntity.ModelComponent.RootPos();
	const float weights[3] = { 1.0f, 10.0f, 1.25f };

	for (const Entity& entity : entities)
	{
		if (!entity.IsValid())
			continue;

		if (!entity.IsTargetable())
			continue;

		int teamFilter = config->aimbotTarget;
		if (!TargetFilter(entity, localEntity, teamFilter))
			continue;

		const Vector3 bone = sdk::GetClosestBone(entity);
		const bool isVisible = entity.getVisibility();

		Vector2 outPos;
		if (!isVisible || !vm.WorldToScreen(bone, &outPos))
			continue;

		const float distanceToCrosshair = (outPos - screenCenter).GetLength();
		if (distanceToCrosshair >= aimbotFOV)
			continue;

		float distance = (entity.ModelComponent.RootPos() - localPosition).Length();
		float life = entity.HealthComponent.Life().Current / entity.HealthComponent.Life().Maximum;

		float entityScore = 0.0f;
		entityScore += weights[0] * distanceToCrosshair;
		entityScore += weights[1] * life;
		entityScore += weights[2] * distance;

		if (entityScore < bestEntityScore)
		{
			bestEntity = entity;
			bestEntityScore = entityScore;
		}
	}

	update::SetBestEntity(bestEntity);
}

bool doOnce = true;
namespace update
{
	const unsigned short MAX_POOL_SIZE = 0x1000;
	const unsigned short MAX_ENTRIES = 250;

	static void ClearAllEntities()
	{
		update::ClearEntities();
		update::SetLocalEntity(Entity(0, 0));
		update::SetBestEntity(Entity(0, 0));
	}

	static void UpdateEntityVisibility(Entity& entity, const Vector3& camera) {
		const Entity& localEntity = update::GetLocalEntity();
		if (!entity.IsValid() || !localEntity.IsValid())
			return;

		Vector3 current(0.f, 0.f, 0.f), previous(0.f, 0.f, 0.f);
		for (const auto& bonegroup : skeleton) {
			for (const auto& bone : bonegroup) {
				current = entity.GetBonePos(bone);

				if (current != previous && previous.x != 0.f) {
					auto rayResult = sdk::RayTrigger(camera, current, config->ignoreShield ? RayCast::IGNORE_SHIELD : RayCast::IGNORE_SHIELD_NEW);

					if (rayResult.entity == entity.GetControllerEntity() && rayResult.entity)
					{
						if (!config->ignoreShield && rayResult.type == RayCast::RaycastReturnType::Entity)
							entity.setVisibility(false);

						if (rayResult.type == RayCast::RaycastReturnType::Entity || rayResult.type == RayCast::RaycastReturnType::DynamicObject)
							entity.setVisibility(true);
					}
				}
				previous = current;
			}
		}
		return;
	}

	static std::vector<Entity> ProcessEntityList(uint64_t entityMapPtr) {
		std::vector<Entity> entityListBuffer;

		Vector3 location = RPM<Vector3>(globals::CameraManager + 0x18);
		location.z = -location.z;

		const uint32_t localPlayerId = RPM<uint32_t>(globals::GameAdmin + Offsets::Engine::LocalPlayerUID);
		if (!IsValidPtr(entityMapPtr))
			return{};

		for (uint32_t i = 0; i < MAX_POOL_SIZE; ++i) {

			Pool currentPool = RPM<Pool>(entityMapPtr + i * sizeof(Pool));

			if (currentPool.size <= 0 || currentPool.address == nullptr || !IsValidPtr((uint64_t)currentPool.address))
				continue;

			for (uint32_t y = 0; y < currentPool.size; ++y) {
				if (!IsValidPtr((uint64_t)currentPool.address[y].address))
					continue;

				PoolEntry entity = currentPool.address[y];

				if (!entity.address || !IsValidPtr(entity.address) || !decrypt::ParentHasComponent(entity.address, TYPE_CONTROLLER))
					continue;

				auto components = decrypt::getEntityComponents(entity.address);

				uint64_t controller = components[TYPE_CONTROLLER];
				if (!controller)
					continue;

				uint64_t controllerEntity = decrypt::GetEntityFromComponent(controller);
				if (!controllerEntity)
					continue;

				Entity currentEntity = Entity(controllerEntity, entity.address);
				if (!currentEntity.IsValid())
					continue;

				if (entity.uid == localPlayerId) {
					update::SetLocalEntity(currentEntity);
					continue;
				}
				else {
					UpdateEntityVisibility(currentEntity, location);

					float ultCharge = currentEntity.StateComponent.GetScriptVariable(0x0, SKILL_ULTIMATE_CHARGE).current_value;
					currentEntity.setUltCharge(ultCharge);
					entityListBuffer.push_back(currentEntity);
				}
			}
		}
		return entityListBuffer;
	}

	static uint32_t entityCount = 0;
	static void UpdateEntities()
	{
		if (!isIngame)
		{
			ClearAllEntities();
			Sleep(1000);
			return;
		}
		decrypt::GetEntityAdmin();

		const uint64_t entityMapPtr = decrypt::GetEntityMap(globals::GameAdmin);
		if (!entityMapPtr)
			return;

		std::vector<Entity> entityBuffer = ProcessEntityList(entityMapPtr);
		entityCount = static_cast<int>(entityBuffer.size());
		update::SetEntities(entityBuffer);
	}

	void Update()
	{
		while (globals::g_Running)
		{
			UpdateIsIngame();
			UpdateEntities();

			if (isIngame)
			{
				Entity localEntity = update::GetLocalEntity();
				if (localEntity.IsValid()) {
					globals::g_PrevHeroName = globals::g_HeroName;
					globals::g_HeroName = localEntity.HeroComponent.GetHeroName();

					if (globals::g_HeroName != globals::g_PrevHeroName)
						globals::g_SaveConfig = true;
					else
						globals::g_SaveConfig = false;
				}
				else
					globals::g_HeroName = "None";

				uint64_t renderData = RPM<uint64_t>(((RPM<uint64_t>(ImageBase + Offsets::Engine::RenderData) + Offsets::Engine::RenderDataKey1) ^ Offsets::Engine::RenderDataKey2) + 0x20);
				globals::ViewMatrix = RPM<uint64_t>(renderData + 0x48) + 0x140;
				globals::CameraManager = RPM<uint64_t>(RPM<uint64_t>(RPM<uint64_t>(RPM<uint64_t>(RPM<uint64_t>(RPM<uint64_t>(ImageBase + Offsets::Engine::CameraManager) + 0x10) + 0x8) + 0x10) + 0x28) + 0x20);
				globals::ViewMatrix2 = RPM<uint64_t>(ImageBase + Offsets::Engine::ViewMatrix) + Offsets::Engine::ViewMatrixPointer;
				CalculateBestEntity();
			}
			else {
				globals::g_SaveConfig = false;
				globals::g_HeroName = "None";
				globals::g_PrevHeroName = "None";
			}
			Sleep(1);
		}
	}
}