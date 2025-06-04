#pragma once
#include <mutex>
#include <shared_mutex>
#include <unordered_set>

struct PoolEntry
{
    uint32_t uid;
    uint32_t pad;
    uint64_t address;
};

struct Pool
{
    PoolEntry* address;
	uint32_t size;
    uint32_t padding;
};

struct PoolEntry2
{
    uint64_t vtable; // 0x0
    uint32_t pad; // 0x8
    uint32_t uid; // 0xC
    uint64_t entity_admin; // 0x10
};

enum EntityIDs : uint64_t {
	PLAYER = 0x40000000000000e,
	BOT = 0x400000000004c49,
	STATIC_AI_PLAYER = 0x4000000000033dd,
	AI_PLAYER = 0x400000000004021,
	HEALTH_PACK_A = 0x400000000000060,
	HEALTH_PACK_B = 0x40000000000005F,
	HEALTH_PACK_C = 0x40000000000480A,
	MOVING_PLATFORM = 0x400000000000516
};

class EntityManager
{
    mutable std::shared_timed_mutex entitiesMutex;
    std::vector<Entity> entities;
    Entity bestEntity;
    Entity localEntity;
public:
    void UpdateEntities(const std::vector<Entity>& newEntities) {
        std::unique_lock<std::shared_timed_mutex> lock(entitiesMutex);
        entities = newEntities;
    }

    std::vector<Entity> GetEntities() const {
        std::shared_lock<std::shared_timed_mutex> lock(entitiesMutex);
        return entities;
    }

    Entity GetBestEntity() const {
        return bestEntity;
    }

    void SetBestEntity(const Entity& newBestEntity) {
        std::unique_lock<std::shared_timed_mutex> lock(entitiesMutex);
        bestEntity = newBestEntity;
    }

    Entity GetLocalEntity() const {
        return localEntity;
    }

    void SetLocalEntity(const Entity& newLocalEntity) {
        std::unique_lock<std::shared_timed_mutex> lock(entitiesMutex);
        localEntity = newLocalEntity;
    }

    void ClearEntities() {
        std::unique_lock<std::shared_timed_mutex> lock(entitiesMutex);
        entities.clear();
    }
};

namespace update {
    inline EntityManager entityManager;
    inline std::atomic_bool isIngame;

    static void SetEntities(std::vector<Entity> entities) {
        entityManager.UpdateEntities(entities);
    }

    static std::vector<Entity> GetEntities() {
        return entityManager.GetEntities();
    }

    static void ClearEntities() {
        entityManager.UpdateEntities({});
    }

    static Entity GetBestEntity() {
        return entityManager.GetBestEntity();
    }

    static void SetBestEntity(Entity bestEntity) {
        entityManager.SetBestEntity(bestEntity);
    }

    static Entity GetLocalEntity() {
        return entityManager.GetLocalEntity();
    }

    static void SetLocalEntity(Entity localEntity) {
        entityManager.SetLocalEntity(localEntity);
    }

    static void UpdateIsIngame() {
        uint64_t singleton_30 = decrypt::GetGlobalPtr(0, 0x30);

        class MapData {
        public:
            void* vTable;
            uint64_t encryptedSingletonList;
            uint8_t singletonIndex;
            uint64_t Map;
            uint64_t Mode;
            uint64_t Mission;
            uint64_t Celebration;
            uint64_t Environment;
        } mapData{};

        if (singleton_30)
            mapData = RPM<MapData>(singleton_30);

        isIngame = mapData.Map > 0;
    }

    static uint64_t GetSelectedHeroID() {
        uint64_t currentlySelectedHero = RPM<uint16_t>(decrypt::GetGlobalPtr(0, 0x28) + 0xC0);
        return currentlySelectedHero;
    }

    extern void Update();
}