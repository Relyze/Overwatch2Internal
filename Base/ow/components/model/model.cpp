#include "../../../includes.h"

Vector3 ModelComponent::RootPos() const
{
	uint64_t component_base = GetComponentBase();
	if (!component_base)
		return Vector3();

	return RPM<Vector3>(component_base + Offsets::Model::RootPosition);
}

Vector3 ModelComponent::Velocity() const
{
	uint64_t component_base = GetComponentBase();
	if (!component_base)
		return Vector3();
	return RPM<Vector3>(component_base + Offsets::Model::Velocity);
}

ModelSkeleton* ModelComponent::GetSkeleton() const
{
	uint64_t component_base = GetComponentBase();
	if (!component_base)
		return nullptr;

	return RPM<ModelSkeleton*>(component_base + Offsets::Model::SkeletalModel);
}

Matrix ModelComponent::WorldMatrix() const {
	__try {
		Matrix worldMatrix;
		if (!IsValid())
			return worldMatrix;

		const uint64_t componentBase = GetComponentBase();
		if (!componentBase)
			return worldMatrix;

		DecomposedMatrix worldMatrixDecomposed;
		std::memcpy(&worldMatrixDecomposed, (void*)(componentBase + Offsets::Model::EncryptedBuffer), 7 * sizeof(float));
		worldMatrix = worldMatrixDecomposed;

		return worldMatrix;
	}
	__except (1) {
		return Matrix();
	}
}

uint16_t ModelComponent::GetBoneIndex(uint32_t index) const
{
	__try {
		if (!IsValid())
			return 0;

		ModelSkeleton* skeleton = GetSkeleton();
		if (skeleton == nullptr || !IsValidPtr((uint64_t)skeleton))
			return 0;

		BoneList* boneList = skeleton->boneList;
		if (boneList == nullptr || !IsValidPtr((uint64_t)boneList))
			return 0;

		uint16_t boneCount = RPM<uint16_t>((uint64_t)skeleton + 0x54);
		if (boneCount <= 0 || !IsValidPtr((uint64_t)boneList + 0x38))
			return 0;

		for (uint16_t i = 0; i < boneCount; i++)
		{
			uint32_t boneValue = boneList->bone_list[i];
			if (boneValue == index)
				return i;
		}
		return 0;
	}
	__except (1) {
		return 0;
	}
}

Matrix ModelComponent::GetBone(uint32_t index) const {
	__try {
		if (!IsValid())
			return Matrix();

		ModelSkeleton* skeleton = GetSkeleton();
		if (skeleton == nullptr || !IsValidPtr((uint64_t)skeleton) || !IsValidPtr((uint64_t)skeleton + 0x28))
			return Matrix();

		uint16_t bonecount = RPM<uint16_t>((uint64_t)skeleton + 0x54);
		if (bonecount <= 0 || bonecount <= index || index <= 0)
			return Matrix();

		return skeleton->bones4x4[index];
	}
	__except (1) {
		return Matrix();
	}
}

Vector3 ModelComponent::GetBonePosition(uint32_t index) const {
	try {

		if (!index || !IsValidPtr((uint64_t)this) || !this->IsValid())
			return Vector3();

		DirectX::XMMATRIX boneMatrix = GetBone(index).ConvertToXMMATRIX();
		DirectX::XMMATRIX worldMatrix = WorldMatrix().ConvertToXMMATRIX();

		DirectX::XMVECTOR boneTranslation = boneMatrix.r[3];
		DirectX::XMVECTOR transformedPosition = DirectX::XMVector3TransformCoord(boneTranslation, worldMatrix);

		transformedPosition = DirectX::XMVectorAdd(transformedPosition, RootPos().ConvertToXMVECTOR());

		return Vector3(DirectX::XMVectorGetX(transformedPosition), DirectX::XMVectorGetY(transformedPosition), DirectX::XMVectorGetZ(transformedPosition));
	}
	catch (...) {
		return Vector3();
	}
}

float ModelComponent::GetLifeTime() const {
	uint64_t componentBase = GetComponentBase();
	if (!IsValid())
		return 0.f;

	float lifeTime = RPM<float>(componentBase + Offsets::Model::LifeTime);
	return lifeTime;
}

void ModelComponent::SetOutline(uint32_t mask, ImColor color) const {
	try {
		if (!IsValid())
			return;

		uint64_t componentBase = GetComponentBase();
		if (!IsValidPtr(componentBase))
			return;

		uint8_t flag = 0x1;

		/*auto fnModelESP = reinterpret_cast<void(__fastcall*)(uint64_t, uint32_t, uint32_t, uint8_t)>(ImageBase + Offsets::Engine::ModelESP);
		auto fnModelFade = reinterpret_cast<void(__fastcall*)(uint64_t, float, float)>(ImageBase + Offsets::Engine::ModelFade);*/

		/*
		    if ( (v1 & 0x2027C1) == mask )
				return;
			*(_DWORD *)(model_component + 0x430) = mask | v1 & 0xFFDFD83F;
		*/

		// untested, todo: just write the struct all at once XD... don't be retarded
		WPM<uint32_t>(componentBase + Offsets::Model::OutlineMask, mask); // pass 0xC1 for esp...
		WPM<uint32_t>(componentBase + Offsets::Model::OutlineColor, ImU32(color));
		WPM<uint8_t>(componentBase + Offsets::Model::OutlineFlag, flag);
		WPM<uint32_t>(componentBase + Offsets::Model::OutlineFadeStart, 0);
		WPM<uint32_t>(componentBase + Offsets::Model::OutlineFadeEnd, 0);
	}
	catch (...) {
		return;
	}
}