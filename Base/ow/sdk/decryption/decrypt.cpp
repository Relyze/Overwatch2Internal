#include "../../../includes.h"

#include "../../../external/include/bdshemu/bddisasm_crt.h"
#include "../../../external/include/bdshemu/inc/bdx86_core.h"

// E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 1D
uint64_t decrypt::GetGlobalPtr(uint64_t admin, uint32_t index)
{
	uint64_t selectedAdmin = admin;
	if (!selectedAdmin)
		selectedAdmin = RPM<uint64_t>(ImageBase + Offsets::Engine::GlobalAdmin);

	uint64_t encrypted_global_pointer_list = RPM<uint64_t>(selectedAdmin + 0x160);

	if (encrypted_global_pointer_list > 0)
	{
		uint64_t global_pointer_list = __ROL8__(__ROL8__(encrypted_singleton_list ^ 0xDE197F6DBC647D48, 0x5) - 0x3E5279DEE667C0FE, 0xB);
		uint64_t global_pointer = RPM<uint64_t>(global_pointer_list + 0x8 * static_cast<uint64_t>(index));
		return global_pointer;
	}
	return 0;
}

// E8 ? ? ? ? 48 8D 15 ? ? ? ? 48 8B CB
// E8 ? ? ? ? 48 8D 15 ? ? ? ? 48 8B CB 4C 8B F8 E8 ? ? ? ? 48 8B 0D
uint64_t decrypt::DecryptPointer(uintptr_t admin, uintptr_t address)
{
	__int64 v4; // rcx
	__int64 v5; // rax
	int v6; // edx
	__int64 v7; // r10
	unsigned __int64 v8; // rax
	unsigned __int64* v10; // rax

	v4 = *(_QWORD*)(admin + 0x198);
	if (!v4)
		goto LABEL_6;
	v5 = address & (*(unsigned int*)(admin + 0x1B0) - 1i64);
	LODWORD(v5) = v5 & 0x7FFFFFFF;
	v6 = *(_DWORD*)(v4 + 4 * v5);
	if (v6 < 0)
		goto LABEL_6;
	v7 = *(_QWORD*)(admin + 0x1A0);
	while (*(_QWORD*)(v7 + 0x10i64 * (unsigned int)v6 + 8) != address)
	{
		v6 = *(_DWORD*)(v7 + 0x10i64 * (unsigned int)v6 + 4);
		if (v6 < 0)
			goto LABEL_6;
	}
	v10 = (unsigned __int64*)(*(_QWORD*)(admin + 0x1A8) + 8i64 * (unsigned int)v6);
	if (v10)
		v8 = *v10;
	else
		LABEL_6:
	v8 = 0x4324E9C6A2BE346DLL;
	return __ROL8__(__ROR8__(v8, 13) - 0x436B63D43FDE4B01LL, 30) ^ 0xC395B2BC17FFAD54uLL;
}

uint64_t decrypt::GetEntityAdmin()
{
	const uint64_t singleton = decrypt::GetGlobalPtr(0, 0x13);
	if (!singleton) return 0;

	struct AdminData { uint64_t vtable; uint32_t pad; uint32_t uid; uint64_t admin; };
	ArrayWrapper<AdminData> adminList = RPM<ArrayWrapper<AdminData>>(singleton + 0x18);

	// This sig sometimes changes, check it if the entity admin ends up being zero once ingame.
	uint64_t result = arrayscan_module(enc("48 8D 05 ? ? ? ? 48 89 03 48 B8 ? ? ? ? ? ? ? ? 48 89 45").c_str(), ImageBase).front(); // prev: 48 8D 05 ? ? ? ? 4C 89 A3 ? ? ? ? 48 89 03
	if (!result) return 0;

	INSTRUX instruction;
	NDSTATUS status = NdDecodeEx(&instruction, reinterpret_cast<const uint8_t*>(result), 16, ND_CODE_64, ND_DATA_64);
	if (status == ND_STATUS_SUCCESS && instruction.Instruction == ND_INS_LEA) {
		uint64_t instructionOffset = result - ImageBase;
		result = ImageBase + instruction.Displacement + instructionOffset + instruction.Length;
	}

	for (const auto& adminData : adminList.asVector()) {
		uint64_t adminVtable = RPM<uint64_t>(adminData.admin);
		if (adminVtable == result) {
			globals::GameAdmin = adminData.admin;
			break;
		}
	}

	return globals::GameAdmin;
}

// 48 89 74 24 ? FF 50 ? 4C 8B 07
uint64_t decrypt::GetEntityMap(uint64_t admin)
{
	if (!admin || RPM<uint8_t>(globals::GameAdmin + 0x8) <= 0)
		return 0;

	const uint64_t encrypted_entity_map = RPM<uint64_t>(admin + 0x18);
	return __ROR8__((__ROR8__(encrypted_entity_map + 0x3C88DAFC02AEE4BC, 0x1) ^ 0xC347AC376DE7CBD5) - 0x1675588A953E51E6, 0x4);
}

uint64_t decrypt::GetEntityFromComponent(uint64_t component)
{
	const uint32_t entity_uid = RPM<uint32_t>(component + Offsets::Component::ParentUID);
	const uint64_t entity_map = decrypt::GetEntityMap(globals::GameAdmin);
	const uint64_t pool_containing_entity_ptr = (uint64_t(0x10) * (entity_uid & 0xFFF)) + entity_map;
	if (!entity_map || !entity_uid)
		return 0;

	const Pool currentPool = RPM<Pool>(pool_containing_entity_ptr);

	for (uint32_t y = 0; y < currentPool.size; ++y) {
		const PoolEntry entity = currentPool.address[y];
		if (entity.uid == entity_uid)
			return entity.address;
	}
	return 0;
}

bool decrypt::ParentHasComponent(uint64_t entity, uint8_t type)
{
	const uint8_t bitIndex = type & 0x3F;
	const uint64_t addressOffset = 8 * (static_cast<uint64_t>(type) >> 6) + 0x110;
	uint64_t value = RPM<uint64_t>(entity + addressOffset);
	return (value >> bitIndex) & 1;
}

std::unordered_map<uint8_t, uint64_t> decrypt::getEntityComponents(uint64_t entity)
{
		const auto dummy = RPM<uint64_t>(ImageBase + 0x36D1758);
		const auto dummy2 = static_cast<uint8_t>(RPM<uint64_t>(ImageBase + 0x33EC32D));
		const auto componentCount = RPM<int>(entity + Offsets::Entity::componentCount);

		std::unordered_map<uint8_t, uint64_t> components;
		if (componentCount <= 0) return components;
		
		uint64_t componentList = RPM<uint64_t>(entity + Offsets::Entity::componentContainer);
		for (uint64_t currentIndex = 0; currentIndex < componentCount; ++currentIndex)
		{
			auto encryptedComponent = RPM<uint64_t>(componentList + 8 * currentIndex);
			encryptedComponent = (__ROL8__(__ROL8__(encryptedComponent ^ 0x28365FF7B976993, 25), 31) - 0x289FECAAF6A63FEC) ^ 0xC3D0077CE1E55F61;
			encryptedComponent = (dummy2 ^ (encryptedComponent - 0x7279928147CCF2F2)) + 0x39DBD2DA38CEEB38;

			uint64_t decryptedComponent = __ROR8__((RPM<uint64_t>(dummy + 0x74) ^ encryptedComponent), 3);
			if (!decryptedComponent)
				continue;

			const uint8_t componentID = RPM<uint8_t>(decryptedComponent + 0x10);
			components[componentID] = decryptedComponent;
		}
		
		return components;
}

std::vector<uintptr_t> decrypt::find_eidolon_gadget_candidates() {
	std::vector<uintptr_t> candidates;
	std::vector<uintptr_t> results = arrayscan_module("48 8B 00 C3", ImageBase);

	for (uintptr_t result : results) {
		if (result > ImageBase)
			candidates.push_back(result);
	}

	return candidates;
}

bool decrypt::find_gadget(uintptr_t& eidolon_gadget) {
	std::vector<uintptr_t> candidates = find_eidolon_gadget_candidates();
	if (candidates.empty()) {
		if(globals::g_Debug)
			printf(enc("Failed to find any eidolon gadgets!\n").c_str());
		return false;
	}
	else {
		eidolon_gadget = candidates.front();
		if (globals::g_Debug)
			printf(enc("Found Eidolon Gadget: %llx\n").c_str(), eidolon_gadget);
	}
	
	return true;
}

uintptr_t decrypt::force_decryption(uintptr_t address) {
	return __forcedecryption((uintptr_t*)address, (uintptr_t*)decrypt::EIDOLON_GADGET);
}
#pragma warning (pop)
