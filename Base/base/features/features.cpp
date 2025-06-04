#include "../../includes.h"

void features::doAim()
{
    while (globals::g_Running)
    {
        const std::vector<Entity> entityList = update::GetEntities();
        const Entity localEntity = update::GetLocalEntity();
        const Entity bestEntity = update::GetBestEntity();
        if (!update::isIngame || entityList.empty() || !bestEntity.IsValid() || !localEntity.IsValid())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        const auto localHero = localEntity.HeroComponent.HeroID();

        int aimKey = config->aimbotKeyID.toInt();
        int triggerKey = config->triggerbotKeyID.toInt();

        if (config->Aimbot)
            aim::doAimbot();
        if (config->Trigger)
            aim::doTriggerbot();
        if (config->SojAutoRightClick && localHero == HERO_SOJOURN)
            misc::doSojournAutoRailgun();

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void features::doMisc()
{
    while (globals::g_Running)
    {
        std::vector<Entity> entities = update::GetEntities();
        const Entity localEntity = update::GetLocalEntity();
        const Entity bestEntity = update::GetBestEntity();

        if (!update::isIngame || entities.empty() || !localEntity.IsValid())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        uint64_t localHero = localEntity.HeroComponent.HeroID();
        if (config->Outline)
            for (const Entity& entity : entities) {
                if (!entity.IsValid())
                    continue;

                bool isEnemy = localEntity.TeamComponent.isEnemy(entity.TeamComponent);
                if(isEnemy)
                    visuals::doOutline(entity, bestEntity);
            }
        else
            for (const Entity& entity : entities) {
                if (!entity.IsValid())
                    continue;

                bool isEnemy = localEntity.TeamComponent.isEnemy(entity.TeamComponent);
                if (isEnemy) {
                    uint32_t outlineFlag{};
                    ImColor outlineColor{};
                    entity.OutlineComponent.GetOutlineInfo(outlineFlag, outlineColor);

                    if(outlineFlag > 0x1)
                        entity.OutlineComponent.SetOutline(0x1, config->OriginalEnemyColor);
                }
            }

        if (config->AutoRecall && localHero == HERO_TRACER)
            misc::doAutoRecall();
        if (config->GenjiAutoDash && localHero == HERO_GENJI)
            misc::doAutoDash();
        if (config->ZenAutoDiscord && localHero == HERO_ZENYATTA)
            misc::doAutoDiscord();
        if (config->AutoMelee)
            misc::doAutoMelee();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void features::doVisuals()
{
    if (!globals::g_Running || !update::isIngame)
        return;

    Entity localEntity = update::GetLocalEntity();
    if (!localEntity.IsValid())
        return;

    const Vector3 localPos = localEntity.ModelComponent.RootPos();
    Matrix vm = sdk::GetMatrix();
    const int screenWidth = render::width;
    const int screenHeight = render::height;

    Entity bestEntity = update::GetBestEntity();
    std::vector<Entity> entities = update::GetEntities();

    for (const Entity& entity : entities)
    {
        if (!entity.IsValid())
            continue;

        bool isEnemy = localEntity.TeamComponent.isEnemy(entity.TeamComponent);
        if (!isEnemy)
            continue;

        Vector3 entityRootPos = entity.ModelComponent.RootPos();
        if (entityRootPos.x == 0.f && entityRootPos.y == 0.f && entityRootPos.z == 0.f)
            continue;

        Vector3 entityHeadPos = entity.GetBonePos(eBoneIndex::forehead);
        Vector2 rootPos, headPos;

        if (vm.WorldToScreen(entityRootPos, &rootPos) && vm.WorldToScreen(entityHeadPos, &headPos))
        {
            float height = abs(headPos.y - rootPos.y);
            float width = height * 0.65f;
            if ((rootPos.x + width * 0.5 >= 0 && rootPos.x + width * 0.5 <= screenWidth && rootPos.y >= 0 && rootPos.y <= screenHeight) || (headPos.x + width * 0.5 >= 0 && headPos.x - width * 0.5 <= screenWidth && headPos.y >= 0 && headPos.y <= screenHeight))
            {
                const float distance = entityRootPos.Distance(localPos);

                if (config->Snaplines)
                    visuals::drawSnapline(entity, bestEntity, headPos, rootPos);
                if (config->Skeleton)
                    visuals::drawSkeleton(entity, bestEntity);
                if (config->Box)
                    visuals::drawBox(entity, bestEntity, headPos, rootPos);
                if (config->healthBar)
                    visuals::drawHealthBar(entity, width, Vector2(headPos.x - (width * 0.5f) - 1, rootPos.y + 5));
                if (config->distanceHeroName || config->BattleTagName)
                    visuals::drawEntityText(entity, rootPos, distance);
            }
        }
    }

    if (!bestEntity.IsValid())
        return;

    if (config->PredEsp)
        visuals::drawPrediction(localEntity, bestEntity);
}
