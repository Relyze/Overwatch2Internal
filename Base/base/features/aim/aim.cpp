#include "../../../includes.h"

namespace aim {
	static std::chrono::high_resolution_clock::time_point lastShotTime = std::chrono::high_resolution_clock::now();
	static auto lastIsHitableTime = std::chrono::high_resolution_clock::now();

	void doAimbot()
	{
		const int aimKey = config->aimbotKeyID.toInt();
		const Entity localEntity = update::GetLocalEntity();
		Entity bestEntity = update::GetBestEntity();
		if (aimKey == 0 || !GetAsyncKeyState(aimKey))
			return;

		const eHero localHero = localEntity.HeroComponent.HeroID();

		Vector3 location = RPM<Vector3>(globals::CameraManager + 0x18);
		location.z = -location.z;

		eBoneIndex bones[] = { eBoneIndex::forehead, eBoneIndex::neck, eBoneIndex::chest, eBoneIndex::pelvis };
		Vector3 bone;

		if (config->boneKeyId <= 0)
			bone = sdk::GetClosestBone(bestEntity);
		else
			bone = bestEntity.GetBonePos(bones[config->boneKeyId + 1]);

		bool shoot_able = true;
		const float distance = location.Distance(bone);
		float speed = sdk::getProjSpeed(localHero);

		bool isPredicted = (speed < 1000.f || localHero == eHero::HERO_SOJOURN || localHero == eHero::HERO_ANA || localHero == eHero::HERO_SIGMA || localHero == eHero::HERO_SOLDIER76);
		bool IsStormArrow = false;
		Vector3 compensation = Vector3(0.f, 0.f, 0.f);
		if (isPredicted)
		{
			float gravity = 0.f;
			if (localHero == eHero::HERO_HANZO)
			{
				if (localEntity.StateComponent.GetScriptVariable(0, eScriptVariableIndex::SKILL_2).writer_count)
					speed = 110.f;
				else
					speed = 25.f + 85.f * 1.0f;

			}

			switch (localHero)
			{
			case eHero::HERO_HANZO:
			case eHero::HERO_TORBJORN:
			case eHero::HERO_JUNKRAT:
				gravity = 9.8f * 0.5f * powf(distance / speed, 2.f);
				break;
			default:
				gravity = 0.f;
				break;
			}

			compensation = bestEntity.ModelComponent.Velocity() * (distance / speed);
			compensation.y += gravity;
			bone += compensation;
		}

		if (shoot_able)
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto timeSinceLastIsHitable = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastIsHitableTime).count();
			int8_t isHitable = sdk::IsHitable(bestEntity, config->boneKeyId, compensation);

			if (isHitable > 0)
			{
				float shotDelay = 0.f;
				if (localHero == eHero::HERO_HANZO && IsStormArrow)
					shotDelay = 290.f;
				else
					shotDelay = sdk::getDelay(localHero);

				auto currentTime = std::chrono::high_resolution_clock::now();
				auto timeSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastShotTime).count();

				if (config->AutoShoot)
				{
					if (localHero == eHero::HERO_HANZO && !IsStormArrow)
						localEntity.ConnectionComponent.SetInput(eInput::INPUT_LMOUSE_UNHOLD, 55);

					if (localHero == eHero::HERO_GENJI) {
						if (bestEntity.ModelComponent.RootPos().Distance(localEntity.ModelComponent.RootPos()) > 10.f)
							localEntity.ConnectionComponent.SetInput(eInput::INPUT_LMOUSE);
						else
							localEntity.ConnectionComponent.SetInput(eInput::INPUT_RMOUSE);
					}

					else if (localHero == eHero::HERO_MAUGA)
					{
						localEntity.ConnectionComponent.SetInput(eInput::INPUT_LMOUSE);
						localEntity.ConnectionComponent.SetInput(eInput::INPUT_RMOUSE);
					}
					else {
						localEntity.ConnectionComponent.SetInput(eInput::INPUT_LMOUSE);
					}

					lastShotTime = std::chrono::high_resolution_clock::now();
				}
			}
			else
			{
				if (!isHitable) {
					Vector3 my_angle = localEntity.ConnectionComponent.Angle();
					Vector3 angle = sdk::CalcAngle(location, bone, my_angle, 1.f);
					Vector3 smoothed_angle = sdk::CalcAngle(location, bone, my_angle, config->aimbotMinSpeed / 100.f);
					Vector3 target_angle = sdk::HumanizeAngle(my_angle, angle, smoothed_angle, distance);
					if ((target_angle.x > -1.f && target_angle.x < 1.f) && (target_angle.y > -1.f && target_angle.y < 1.f) && (target_angle.z > -1.f && target_angle.z < 1.f))
						localEntity.ConnectionComponent.SetAngle(target_angle);
				}
			}
		}
	}

	void doTriggerbot()
	{
		const int triggerKey = config->triggerbotKeyID.toInt();
		if (!GetAsyncKeyState(triggerKey) || triggerKey == 0)
			return;

		Entity localEntity = update::GetLocalEntity();
		Entity bestEntity = update::GetBestEntity();
		const eHero localHero = localEntity.HeroComponent.HeroID();
		bool IsStormArrow = localHero == eHero::HERO_HANZO && localEntity.StateComponent.GetScriptVariable(0, eScriptVariableIndex::SKILL_2).writer_count;

		Vector3 bone = sdk::GetClosestBone(bestEntity);
		uint16_t bone_id = sdk::GetClosestBoneID(bestEntity);

		if (sdk::IsHitable(bestEntity, bone_id) <= 0)
			return;

		float shotDelay = IsStormArrow ? 290.f : sdk::getDelay(localHero);

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto timeSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastShotTime).count();
		if (timeSinceLastShot > shotDelay)
		{
			if (localHero == eHero::HERO_HANZO && !IsStormArrow)
				localEntity.ConnectionComponent.SetInput(eInput::INPUT_LMOUSE_UNHOLD, 55);
			else if (localHero == eHero::HERO_GENJI && bestEntity.ModelComponent.RootPos().Distance(localEntity.ModelComponent.RootPos()) > 10.f)
				localEntity.ConnectionComponent.SetInput(eInput::INPUT_LMOUSE);
			else
				localEntity.ConnectionComponent.SetInput(config->Left ? eInput::INPUT_LMOUSE : eInput::INPUT_RMOUSE);

			lastShotTime = std::chrono::high_resolution_clock::now();
		}
	}
}