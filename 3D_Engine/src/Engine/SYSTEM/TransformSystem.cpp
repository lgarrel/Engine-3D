#include "pch.h"
#include "TransformSystem.h"

TransformSystem::TransformSystem()
{
}

void TransformSystem::Update(Scene* currentScene, float deltaTime)
{
    std::vector<Entity*> allEntities = currentScene->GetEntitiesWithComponent<TransformComponent>();

    for (Entity* entity : allEntities)
    {
        if (entity == nullptr)
            continue;

        if(entity->HasParent() == false)
        {
            UpdateEntity(entity);
        }
    }
}

void TransformSystem::UpdateEntity(Entity* entity)
{
    if (entity == nullptr)
        return;

    TransformComponent* tc = entity->GetComponent<TransformComponent>();
    if (tc != nullptr)
    {
        Transform& t = tc->GetTransform();
        t.UpdateLocal();

        if (entity->HasParent())
        {
            TransformComponent* parentTc = entity->GetParent()->GetComponent<TransformComponent>();
            if (parentTc != nullptr)
            {
                t.ApplyParentMatrix(parentTc->GetWorldMatrix());
            }
        }
        else
        {
            
        }
    }

    for (Entity* child : entity->GetChilds())
    {
        UpdateEntity(child);
    }
}