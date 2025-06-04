#include "../../includes.h"

namespace sdk {
	Matrix GetMatrix()
	{
		Matrix matrix = RPM<Matrix>(globals::ViewMatrix2);
		return matrix;
	}

	Matrix GetProjectMatrix()
	{
		return RPM<Matrix>(globals::ViewMatrix2 + sizeof(Matrix));
	}

	uint64_t GetEntityDefinitionID(uint64_t entity)
	{
		uint64_t uid = RPM<uint64_t>((RPM<uint64_t>(entity + Offsets::Entity::unalignedSpecifier) & 0xFFFFFFFFFFFFFFC0) + 0x10);
		return uid;
	}

	float getProjSpeed(eHero HeroID)
	{
		switch (HeroID)
		{
		case eHero::HERO_REAPER:
			return 9999.f;
		case eHero::HERO_TRACER:
			return 9999.f;
		case eHero::HERO_MERCY:
			return 50.f;
		case eHero::HERO_HANZO:
			return 109.f;
		case eHero::HERO_TORBJORN:
			return 70.f;
		case eHero::HERO_REINHARDT:
			return 9999.f;
		case eHero::HERO_PHARAH:
			return 35.f;
		case eHero::HERO_WINSTON:
			return 9999.f;
		case eHero::HERO_WIDOWMAKER:
			return 9999.f;
		case eHero::HERO_BASTION:
			return 9999.f;
		case eHero::HERO_SYMMETRA:
			return 9999.f;
		case eHero::HERO_ZENYATTA:
			return 90.f;
		case eHero::HERO_GENJI:
			return 60.f;
		case eHero::HERO_ROADHOG:
			return 60.f;
		case eHero::HERO_CASSIDY:
			return 9999.f;
		case eHero::HERO_JUNKRAT:
			return 25.f;
		case eHero::HERO_ZARYA:
			return 9999.f;
		case eHero::HERO_SOLDIER76:
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				return 9999.f;
			}
			else
			{
				return 50.f;
			}
		case eHero::HERO_LUCIO:
			return 50.f;
		case eHero::HERO_DVA:
			return 50.f;
		case eHero::HERO_MEI:
			return 9999.f;
		case eHero::HERO_ANA:
			if (GetAsyncKeyState(VK_RBUTTON))
			{
				return 9999.f;
			}
			else
			{
				return 140.f;
			}
		case eHero::HERO_SOMBRA:
			return 9999.f;
		case eHero::HERO_ORISA:
			return 120.f;
		case eHero::HERO_DOOMFIST:
			return 80.f;
		case eHero::HERO_MOIRA:
			return 9999.f;
		case eHero::HERO_BRIGITTE:
			return 9999.f;
		case eHero::HERO_WRECKINGBALL:
			return 9999.f;
		case eHero::HERO_ASHE:
			return 9999.f;
		case eHero::HERO_BAPTISTE:
			return 9999.f;
		case eHero::HERO_SIGMA:
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				return 60.f;
			}
			else
			{
				return 37.5f;
			}
		case eHero::HERO_ECHO:
			return 75.f;
		case eHero::HERO_TRAININGBOT1:
			return 9999.f;
		case eHero::HERO_TRAININGBOT2:
			return 9999.f;
		case eHero::HERO_TRAININGBOT3:
			return 9999.f;
		case eHero::HERO_TRAININGBOT4:
			return 9999.f;
		case eHero::HERO_SOJOURN:
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				return 160.f;

			}
			else
			{
				return 99999.f;
			}

		case eHero::HERO_KIRIKO:
			return 70.f;
		case eHero::HERO_JUNKERQUEEN:
			return 99999.f;
		case eHero::HERO_RAMATTRA:
			return 80.f;
		case eHero::HERO_LIFEWEAVER:
			return 80.f;
		default:
			return 9999.f;
		}
	}

	float getDelay(uint64_t HeroID)
	{
		switch (HeroID)
		{
		case eHero::HERO_REAPER:
			return 500.f;
		case eHero::HERO_TRACER:
			return 0.f;
		case eHero::HERO_MERCY:
			return 200.f;
		case eHero::HERO_HANZO:
			return 251.f;
		case eHero::HERO_JUNKERQUEEN:
			return 750.f;
		case eHero::HERO_KIRIKO:
			return 550.f;
		case eHero::HERO_TORBJORN:
			return 600.f;
		case eHero::HERO_SOJOURN:
			return 750.f;
		case eHero::HERO_REINHARDT:
			return 200.f;
		case eHero::HERO_PHARAH:
			return 850.f;
		case eHero::HERO_WINSTON:
			return 200.f;
		case eHero::HERO_WIDOWMAKER:
			return 500.f;
		case eHero::HERO_BASTION:
			return 200.f;
		case eHero::HERO_SYMMETRA:
			return 200.f;
		case eHero::HERO_ZENYATTA:
			return 400.f;
		case eHero::HERO_GENJI:
			return 695.f;
		case eHero::HERO_ROADHOG:
			return 200.f;
		case eHero::HERO_CASSIDY: // 2.02 shots per second -> 495.05ms per shot
			return 496.f;
		case eHero::HERO_JUNKRAT:
			return 630.f;
		case eHero::HERO_ZARYA:
			return 200.f;
		case eHero::HERO_SOLDIER76:
			return 0.f;
		case eHero::HERO_LUCIO:
			return 200.f;
		case eHero::HERO_DVA:
			return 200.f;
		case eHero::HERO_MEI:
			return 800.f;
		case eHero::HERO_ANA:
			return 800.f;
		case eHero::HERO_SOMBRA:
			return 200.f;
		case eHero::HERO_ORISA:
			return 200.f;
		case eHero::HERO_DOOMFIST:
			return 355.f;
		case eHero::HERO_MOIRA:
			return 200.f;
		case eHero::HERO_BRIGITTE:
			return 200.f;
		case eHero::HERO_WRECKINGBALL:
			return 200.f;
		case eHero::HERO_ASHE:
			if (GetAsyncKeyState(VK_RBUTTON))
			{
				return 650.f;
			}
			else
			{
				return 250.f;
			}
		case eHero::HERO_BAPTISTE:
			return 200.f;
		case eHero::HERO_SIGMA:
			return 200.f;
		case eHero::HERO_ECHO:
			return 335.f;
		case eHero::HERO_TRAININGBOT1:
			return 9999999.f;
		case eHero::HERO_TRAININGBOT2:
			return 9999999.f;
		case eHero::HERO_TRAININGBOT3:
			return 9999999.f;
		case eHero::HERO_TRAININGBOT4:
			return 9999999.f;
		default:
			return 500.f;
		}
	}

	Vector3 CalcAngle(Vector3 MyPos, Vector3 EnPos)
	{
		Vector3 Result;

		float Dis = MyPos.Distance(EnPos);
		Result.x = (EnPos.x - MyPos.x) / Dis;
		Result.y = (EnPos.y - MyPos.y) / Dis;
		Result.z = (EnPos.z - MyPos.z) / Dis;

		return Result;
	}

	Vector3 CalcAngle(Vector3 cameraPos, Vector3 targetPos, Vector3 currentLookDir, float speed)
	{
		// Ensure speed is in a reasonable range
		speed = std::clamp(speed, 0.0f, 1.0f);

		// Calculate the normalized direction vector toward the target
		Vector3 desiredLookDir;
		float distance = cameraPos.Distance(targetPos);
		desiredLookDir.x = (targetPos.x - cameraPos.x) / distance;
		desiredLookDir.y = (targetPos.y - cameraPos.y) / distance;
		desiredLookDir.z = (targetPos.z - cameraPos.z) / distance;

		// Calculate the adjustment vector
		Vector3 lookAdjustment = desiredLookDir - currentLookDir;

		// If adjustment is very small, we are already aligned
		if (lookAdjustment.Length() < 0.001f)
			return desiredLookDir;

		// Return the interpolated look direction
		return currentLookDir + lookAdjustment * speed;
	}

	Vector3 HumanizeAngle(Vector3 my_angle, Vector3 angle, Vector3 smoothed_angle, float distance)
	{
		Vector3 delta = angle - my_angle;
		Vector3 smoothed_delta = smoothed_angle - my_angle;

		float percent = 1 + distance / 100.f;
		float min_speed = config->aimbotSpeed / 50000.f / percent;

		float y_delta = 0.f;
		if (fabs(delta.y) < (fabs(delta.z) + fabs(delta.x)) / 3.f)
		{
			y_delta = min_speed / 1.5f;
			smoothed_delta.y /= 1.5f;
		}
		else
		{
			y_delta = min_speed * 1.3f;
			smoothed_delta.y *= 1.3f;
		}

		if (delta.x > 0)
			delta.x = min(smoothed_delta.x, min_speed);
		else
			delta.x = -min(-smoothed_delta.x, min_speed);

		if (delta.y > 0)
			delta.y = min(smoothed_delta.y, y_delta);
		else
			delta.y = -min(-smoothed_delta.y, y_delta);

		if (delta.z > 0)
			delta.z = min(smoothed_delta.z, min_speed);
		else
			delta.z = -min(-smoothed_delta.z, min_speed);

		return my_angle + delta;
	}

	uint16_t GetClosestBoneID(Entity entity)
	{
		Vector3 location = RPM<Vector3>(globals::CameraManager + 0x18);
		location.z = -location.z;
		uint32_t closestPos = 0x0;

		const Entity localEntity = update::GetLocalEntity();
		if (!localEntity.IsValid())
			return closestPos;

		Matrix projectionMatrix = sdk::GetProjectMatrix();
		DirectX::XMMATRIX worldViewMatrix = sdk::GetProjectMatrix().ConvertToXMMATRIX();

		DirectX::XMVECTOR scale, rotation, translation;
		DirectX::XMMatrixDecompose(&scale, &rotation, &translation, worldViewMatrix);

		rotation = DirectX::XMQuaternionNormalize(rotation);

		Vector3 rotationVec = Vector3(DirectX::XMVectorGetX(rotation), DirectX::XMVectorGetY(rotation), DirectX::XMVectorGetZ(rotation));
		float minDistance = (std::numeric_limits<float>::max)();

		for (const auto& a : skeleton)
		{
			for (const auto& bone : a)
			{
				Vector3 bonePos = entity.GetBonePos(bone);
				float currentDistance = rotationVec.Distance(CalcAngle(location, bonePos, rotationVec, true));

				if (currentDistance < minDistance)
				{
					minDistance = currentDistance;
					closestPos = entity.GetBoneID(bone);
				}
			}
		}
		return closestPos;
	}

	Vector3 GetClosestBone(Entity entity)
	{
		Vector3 location = RPM<Vector3>(globals::CameraManager + 0x18);
		location.z = -location.z;

		Vector3 closestPos(0.0f, 0.0f, 0.0f);

		const Entity localEntity = update::GetLocalEntity();
		if (!localEntity.IsValid())
			return closestPos;

		Matrix projectionMatrix = sdk::GetProjectMatrix();
		DirectX::XMMATRIX worldViewMatrix = sdk::GetProjectMatrix().ConvertToXMMATRIX();

		DirectX::XMVECTOR scale, rotation, translation;
		DirectX::XMMatrixDecompose(&scale, &rotation, &translation, worldViewMatrix);

		// Normalize the rotation quaternion
		rotation = DirectX::XMQuaternionNormalize(rotation);

		Vector3 rotationVec = Vector3(DirectX::XMVectorGetX(rotation), DirectX::XMVectorGetY(rotation), DirectX::XMVectorGetZ(rotation));
		float minDistance = (std::numeric_limits<float>::max)();

		for (const std::list<eBoneIndex>& section : skeleton)
		{
			for (const uint32_t& bone : section)
			{
				Vector3 bonePos = entity.GetBonePos(bone);
				float currentDistance = rotationVec.Distance(CalcAngle(location, bonePos, rotationVec, 1.0f));

				if (currentDistance < minDistance)
				{
					minDistance = currentDistance;
					closestPos = bonePos;
				}
			}
		}

		return closestPos;
	}
	
	bool RayTrace(Vector3 from, Vector3 to)
	{
		const Entity localEntity = update::GetLocalEntity();
		if (!localEntity.IsValid())
			return false;

		const uint8_t adminValid = RPM<uint8_t>(globals::GameAdmin + 0x8);
		if (!IsValidPtr(globals::GameAdmin))
			return false;

		const uint64_t local = localEntity.GetControllerEntity();
		if (!IsValidPtr(local))
			return false;

		if (adminValid && IsValidPtr(globals::GameAdmin)) {

			RayCast::Raycaster raycaster(globals::GameAdmin, RayCast::BASIC_MASK);

			if (IsValidPtr(local) && localEntity.IsValid())
				raycaster.ignored.push_back(local);

			RayCast::RaycastResult result = raycaster.trace(from, to);
			return result.type == RayCast::Entity;
		}

		return false;
	}

	RayCast::RaycastResult RayTrigger(Vector3 from, Vector3 to, RayCast::RayMask mask)
	{
		RayCast::RaycastResult result = { RayCast::RaycastReturnType::None, Vector3(), Vector3(), 0x0, 0x0 };
		if (std::isnan(from.x) || std::isnan(from.y) || std::isnan(from.z) || std::isnan(to.x) || std::isnan(to.y) || std::isnan(to.z))
			return result;

		const Entity localEntity = update::GetLocalEntity();
		if (!localEntity.IsValid())
			return result;

		const uint8_t adminValid = RPM<uint8_t>(globals::GameAdmin + 0x8);
		if (!IsValidPtr(globals::GameAdmin))
			return result;

		const uint64_t local = localEntity.GetControllerEntity();
		if (!IsValidPtr(local))
			return result;

		if (adminValid && IsValidPtr(globals::GameAdmin)) {
			RayCast::Raycaster raycaster(globals::GameAdmin, mask);

			if (IsValidPtr(local) && localEntity.IsValid())
				raycaster.ignored.push_back(local);

			result = raycaster.trace(from, to);

			//printf("Raycast Result:\n");
			//printf("\tfrom: %.2f %.2f %.2f", from.x, from.y, from.z);
			//printf("\tto: %.2f %.2f %.2f", to.x, to.y, to.z);
			//printf("\ttype: %d\n", result.type);
			//printf("\tlocation: %.2f %.2f %.2f\n", result.location.x, result.location.y, result.location.z);
			//printf("\tnormal: %.2f %.2f %.2f\n", result.normal.x, result.normal.y, result.normal.z);
			//printf("\tentity: %llx\n", result.entity);
			//printf("\thb_guid: %llx\n", result.hb_guid);

			return result;
		}

		return result;
	}

	int8_t IsHitable(Entity& targetEntity, uint32_t targetBone, Vector3 compensation)
	{
		Entity localEntity = update::GetLocalEntity();

		// Check if entities are valid
		if (!localEntity.IsValid() || !targetEntity.IsValid())
			return false;

		uint64_t local = localEntity.GetControllerEntity();
		uint64_t target = targetEntity.GetControllerEntity();

		Vector3 location = RPM<Vector3>(globals::CameraManager + 0x18);
		location.z = -location.z;

		Matrix projectionMatrix = sdk::GetProjectMatrix();
		DirectX::XMMATRIX worldViewMatrix = sdk::GetProjectMatrix().ConvertToXMMATRIX();

		DirectX::XMVECTOR scale, rotation, translation;
		DirectX::XMMatrixDecompose(&scale, &rotation, &translation, worldViewMatrix);

		DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		rotation = DirectX::XMQuaternionInverse(rotation);
		Vector3 direction = (Vector3(DirectX::XMVector3Rotate(vec, rotation))).Normalize();

		const float distance = 300.f;
		Vector3 to = location + (direction * distance);

		auto rayResult = RayTrigger(location - compensation, to + compensation, config->ignoreShield ? RayCast::IGNORE_SHIELD : RayCast::IGNORE_SHIELD_NEW);
		
		if (rayResult.entity == target && rayResult.entity)
		{
			if (!config->ignoreShield && rayResult.type == RayCast::RaycastReturnType::Entity)
				return false;

			if (rayResult.type == RayCast::RaycastReturnType::Entity || rayResult.type == RayCast::RaycastReturnType::DynamicObject)
				return (rayResult.hb_guid == 0xDA000000000000D) ? 2 : 1;
		}

		return false;
	}
}