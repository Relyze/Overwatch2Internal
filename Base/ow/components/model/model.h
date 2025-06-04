#pragma once

enum eBoneIndex
{
	forehead,
	neck,
	chest,
	pelvis,
	upperarm_l,
	lowerarm_l,
	hand_l,
	upperarm_r,
	lowerarm_r,
	hand_r,
	upperleg_l,
	lowerleg_l,
	foot_l,
	upperleg_r,
	lowerleg_r,
	foot_r
};

static std::list<eBoneIndex> upper_part = { neck, forehead };
static std::list<eBoneIndex> right_arm = { neck, upperarm_r, lowerarm_r, hand_r };
static std::list<eBoneIndex> left_arm = { neck, upperarm_l, lowerarm_l, hand_l };
static std::list<eBoneIndex> spine = { neck, chest, pelvis };
static std::list<eBoneIndex> lower_right = { pelvis, upperleg_r, lowerleg_r, foot_r };
static std::list<eBoneIndex> lower_left = { pelvis, upperleg_l, lowerleg_l, foot_l };
static std::list<std::list<eBoneIndex>> skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };

// forehead, neck, chest, pelvis, upperarm_l, lowerarm_l, hand_l, upperarm_r, lowerarm_r, hand_r, upperleg_l, lowerleg_l, foot_l, upperleg_r, lowerleg_r, foot_r
// 105,        82,    82,     82,         80,         13,     13,         53,         54,     54,         82,         82,     82,         82,         82,     82
struct BoneList {
	char padding_0[0x38]; // 0x000
	uint32_t* bone_list;   // 0x038
	char padding_40[0x24]; // 0x036
	uint16_t bone_count1;  // 0x064
	uint16_t bone_count2;   // 0x066
};

struct ModelSkeleton {
	BoneList* boneList; // 0x0
private: char pad_8[0x18]; public:
	DecomposedMatrix* bones; // 0x20
	Matrix* bones4x4; // 0x28
private: char pad_30[0x24]; public:
	uint16_t bone_count; // 0x54
}; // Size: 0x68

class ModelComponent : public Component<TYPE_MODEL>
{
public:
	ModelComponent(uint64_t ComponentParent = 0) : Component<TYPE_MODEL>(ComponentParent) {}

	Matrix WorldMatrix() const;
	Vector3 RootPos() const;
	Vector3 Velocity() const;
	ModelSkeleton* GetSkeleton() const;
	Matrix GetBone(uint32_t index) const;
	uint16_t GetBoneIndex(uint32_t index) const;
	Vector3 GetBonePosition(uint32_t index) const;
	float GetLifeTime() const;
	void SetOutline(uint32_t mask, ImColor color) const;
};