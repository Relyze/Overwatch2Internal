#pragma once
#include "../includes.h"
#include <DirectXMath.h>

class EntityRef {
	EntityRef* parentEntity; // 0x0
	EntityRef* childEntity; // 0x8
	EntityRef* siblingEntity; // 0x10
private: char pad_18[0x10]; public:
	uint64_t encryptedAdmin; // 0x28
	uint64_t unalignedSpecifier; // 0x30
private: char pad_38[0x48]; public:
	uint64_t* componentContainer; // 0x80
private: char pad_88[0x88]; public:
	uint64_t allocationBitmaps; // 0x110
private: char pad_118[0x18]; public:
	uint64_t segmentOffsets; // 0x130
	uint64_t entityID; // 0x138
}; // Size: 0x140

class Entity
{
	uint64_t controller_entity_ = 0;
	uint64_t entity_ = 0;
	uint32_t uid_ = 0;
	bool isVisible = false;
	float ultCharge = 0.f;
	mutable std::mutex entityMutex;

	// Helper function for swapping components
	template <typename T>
	static void swapComponents(T& a, T& b) {
		std::lock_guard<std::mutex> lock(a.entityMutex);
		std::swap(a, b);
	}
public:
	Entity() = default;
	Entity(uint64_t controller, uint64_t entity) noexcept
		: controller_entity_(controller),
		entity_(entity),
		uid_(RPM<uint32_t>(entity + Offsets::Entity::entityID)) {}

	// Copy constructor to handle copying of mutexes
	Entity(const Entity& other)
		: controller_entity_(other.controller_entity_),
		entity_(other.entity_),
		uid_(other.uid_),
		isVisible(other.isVisible),
		ModelComponent(other.ModelComponent),  // Copy ModelComponent
		HealthComponent(other.HealthComponent),  // Copy HealthComponent
		StateComponent(other.StateComponent),  // Copy StateComponent
		OutlineComponent(other.OutlineComponent),  // Copy OutlineComponent
		ControllerComponent(other.ControllerComponent),  // Copy ControllerComponent
		ConnectionComponent(other.ConnectionComponent),  // Copy ConnectionComponent
		TagComponent(other.TagComponent),  // Copy TagComponent
		HeroComponent(other.HeroComponent),  // Copy HeroComponent
		TeamComponent(other.TeamComponent)  // Copy TeamComponent
	{}

	// Copy assignment operator to handle copying of mutexes
	Entity& operator=(const Entity& other)
	{
		if (this != &other) {
			controller_entity_ = other.controller_entity_;
			entity_ = other.entity_;
			uid_ = other.uid_;
			isVisible = other.isVisible;

			// Copy non-copyable members
			ModelComponent = other.ModelComponent;
			HealthComponent = other.HealthComponent;
			StateComponent = other.StateComponent;
			OutlineComponent = other.OutlineComponent;

			ControllerComponent = other.ControllerComponent;
			ConnectionComponent = other.ConnectionComponent;
			TagComponent = other.TagComponent;
			HeroComponent = other.HeroComponent;
			TeamComponent = other.TeamComponent;
		}
		return *this;
	}

	bool operator==(const Entity& entity) const noexcept {
		return entity.controller_entity_ == controller_entity_;
	}

	bool operator!=(const Entity& entity) const noexcept {
		return !(*this == entity);
	}

	// Getter and setter methods
	uint64_t GetControllerEntity() const noexcept {
		return controller_entity_;
	}

	void SetControllerEntity(uint64_t controller) noexcept {
		controller_entity_ = controller;
	}

	uint64_t GetEntity() const noexcept {
		return entity_;
	}

	void SetEntity(uint64_t entity) noexcept {
		entity_ = entity;
	}

	uint32_t GetUID() const noexcept {
		return uid_ ? uid_ : -1;
	}

	void SetUID(uint32_t uid) noexcept {
		uid_ = uid;
	}

	bool getVisibility() const noexcept {
		return isVisible;
	}

	void setVisibility(bool visibility) noexcept {
		isVisible = visibility;
	}

	float getUltCharge() const noexcept {
		return ultCharge;
	}

	void setUltCharge(float charge) noexcept {
		ultCharge = charge;
	}

	mutable ModelComponent ModelComponent;
	mutable HealthComponent HealthComponent;
	mutable StateComponent StateComponent;
	mutable OutlineComponent OutlineComponent;

	mutable ControllerComponent ControllerComponent;
	mutable ConnectionComponent ConnectionComponent;
	mutable TagComponent TagComponent;
	mutable HeroComponent HeroComponent;
	mutable TeamComponent TeamComponent;

	bool IsValid() const {
		if (RPM<uint64_t>(entity_ - 0x8) != -1 && RPM<uint64_t>(controller_entity_ - 0x8) != -1 && RPM<uint32_t>(entity_ + Offsets::Entity::componentCount) > 0)
		{
			// Update components associated with the controller entity.
			const auto controllerEntityComponents = decrypt::getEntityComponents(controller_entity_);
			const auto baseEntityComponents = decrypt::getEntityComponents(entity_);
			HealthComponent.Update(controllerEntityComponents);
			ModelComponent.Update(controllerEntityComponents);
			HeroComponent.Update(baseEntityComponents);
			TeamComponent.Update(baseEntityComponents);

			if (!HealthComponent.IsValid() || !HealthComponent.IsAlive() || !ModelComponent.IsValid() || !HeroComponent.IsValid() || !TeamComponent.IsValid())
				return false;

			StateComponent.Update(controllerEntityComponents);
			OutlineComponent.Update(controllerEntityComponents);

			// Update components associated with the entity.
			ControllerComponent.Update(baseEntityComponents);
			ConnectionComponent.Update(baseEntityComponents); // Local player exclusive.
			TagComponent.Update(baseEntityComponents);         // Player exclusive.

			return true;
		}
		return false;
	}

	bool IsTargetable() const {
		switch (HeroComponent.HeroID())
		{
		case eHero::HERO_GENJI:
			return !StateComponent.GetScriptVariable(0, eScriptVariableIndex::SKILL_1).writer_count && !StateComponent.GetScriptVariable(0, eScriptVariableIndex::SKILL_2).writer_count;
		case eHero::HERO_ZARYA:
		case eHero::HERO_SOMBRA:
		case eHero::HERO_REAPER:
		case eHero::HERO_MEI:
		case eHero::HERO_MOIRA:
		case eHero::HERO_TRACER:
		case eHero::HERO_SIGMA:
			return !StateComponent.GetScriptVariable(0, eScriptVariableIndex::SKILL_1).writer_count;
		default:
			return true;
		}

		return false;
	}

	uint32_t GetBoneID(uint32_t Index) const
	{
		eHero heroID = HeroComponent.HeroID();
		RefreshBone(heroID);
		if (Index < 16)
			return skeleton[Index];
		return 0;
	}

	Vector3 GetBonePos(uint32_t index) const {

		if (!ModelComponent.IsValid())
			return Vector3();

		uint32_t boneID = GetBoneID(index);

		// Calculate the position of the bone and apply rotation to it
		uint32_t boneIndex = ModelComponent.GetBoneIndex(boneID);
		if (boneIndex <= 0)
			return Vector3();

		Vector3 bonePos = ModelComponent.GetBonePosition(boneIndex);

		// Return the final position including the root position of the model
		return bonePos;
	}

	mutable std::array<uint32_t, 16> skeleton{};

private:
	// forehead, neck, chest, pelvis, upperarm_l, lowerarm_l, hand_l, upperarm_r, lowerarm_r, hand_r, upperleg_l, lowerleg_l, foot_l, upperleg_r, lowerleg_r, foot_r
	// 105,        82,    82,     82,         80,         13,     13,         53,         54,    54,         82,          82,     82,         82, 82, 82
	void RefreshBone(eHero hero) const {
		// Check if the skeleton is already initialized
		if (!skeleton[0]) {
			// Define a map to store bone indices for different heroes
			std::map<eHero, std::array<uint32_t, 16>> heroBoneIndices;

			// Define default bone indices
			std::array<uint32_t, 16> defaultBoneIndices = { 17, 16, 15, 2, 13, 14, 28, 54, 55, 58, 85, 89, 90, 95, 99, 100 };

			// Set bone indices for specific heroes
			heroBoneIndices[eHero::HERO_BASTION] = { 17, 16, 4, 3, 13, 14, 44, 54, 55, 145, 89, 282, 90, 99, 283, 100 };
			heroBoneIndices[eHero::HERO_TRAININGBOT1] = heroBoneIndices[eHero::HERO_TRAININGBOT2] = heroBoneIndices[eHero::HERO_TRAININGBOT5] = heroBoneIndices[eHero::HERO_TRAININGBOT6] = { 105, 82, 82, 82, 80, 13, 386, 53, 54, 387, 82, 82, 82, 82, 82, 82 };
			heroBoneIndices[eHero::HERO_TRAININGBOT4] = heroBoneIndices[eHero::HERO_TRAININGBOT3] = { 105, 82, 82, 82, 80, 13, 13, 53, 54, 54, 82, 82, 82, 82, 82, 82 };
			if (hero == eHero::HERO_DVA)
			{
				if (ModelComponent.IsValid()) {
					ModelSkeleton* skeleton = ModelComponent.GetSkeleton();
					if (skeleton == nullptr || !IsValidPtr((uint64_t)skeleton) || !IsValidPtr((uint64_t)skeleton + 0x28) || !IsValidPtr((uint64_t)skeleton + 0x20))
						return;

					uint16_t boneCount = skeleton->bone_count;
					if (boneCount > 240)
						heroBoneIndices[eHero::HERO_DVA] = { 17, 16, 3, 2, 13, 14, 28, 54, 55, 58, 85, 89, 282, 95, 99, 283 };
					else
						heroBoneIndices[eHero::HERO_DVA] = defaultBoneIndices;
				}
			}

			// Set the skeleton based on the hero
			if (heroBoneIndices.find(hero) != heroBoneIndices.end())
				skeleton = heroBoneIndices[hero];
			else
				skeleton = defaultBoneIndices;
		}
	}
};