#pragma once
#include <unordered_map>
#include <optional>
#include <type_traits>

extern "C" uintptr_t __forcedecryption(uintptr_t* a, uintptr_t* b);

namespace decrypt
{
	inline uint64_t EIDOLON_GADGET;
    uint64_t GetGlobalPtr(uint64_t admin, uint32_t index);
    uint64_t DecryptPointer(uint64_t admin, uint64_t address);
	uint64_t GetEntityAdmin();
	uint64_t GetEntityMap(uint64_t admin);
	uint64_t GetEntityFromComponent(uint64_t component);
	bool ParentHasComponent(uint64_t parent, uint8_t type);
	std::unordered_map<uint8_t, uint64_t> getEntityComponents(uint64_t entity);

	bool find_gadget(uintptr_t& eidolon_gadget);
    std::vector<uintptr_t> find_eidolon_gadget_candidates();
	uintptr_t force_decryption(uintptr_t address);
}