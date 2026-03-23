#include "pch.h"
#include "LightSystem.h"

void LightSystem::Update(Scene* scene, float deltaTime)
{
}

void LightSystem::FillPassConstants(Scene* scene, PassConstants& passCb)
{
    if (!scene) 
        return;

    auto lightEntities = scene->GetEntitiesWithComponent<LightComponent>();

    int dirLightCount = 0;
    int pointLightCount = 0;
    int spotLightCount = 0;

    for (int i = 0; i < 16; ++i)
    {
        passCb.Lights[i].m_strength = { 0.0f, 0.0f, 0.0f };
    }

    std::vector<LightComponent*> dirLights;
    std::vector<LightComponent*> pointLights;
    std::vector<LightComponent*> spotLights;

    for (Entity* entity : lightEntities)
    {
        LightComponent* lightComp = entity->GetComponent<LightComponent>();
        if (!lightComp) 
            continue;

        TransformComponent* tc = entity->GetComponent<TransformComponent>();
        if (tc)
        {
            lightComp->GetLightData().m_position = tc->GetTransform().m_worldPosition;
            lightComp->GetLightData().m_direction = tc->GetTransform().m_forward;
        }

        switch (lightComp->GetType())
        {
        case LightComponent::LightType::Directional:
            dirLights.push_back(lightComp); 
            break;

        case LightComponent::LightType::Point: 
            pointLights.push_back(lightComp); 
            break;

        case LightComponent::LightType::Spot: 
            spotLights.push_back(lightComp); 
            break;
        }
    }

    int lightIndex = 0;
    for (auto* light : dirLights)
    {
        if (lightIndex < 16)
        {
            passCb.Lights[lightIndex++] = light->GetLightData();
        }
    }

    for (auto* light : pointLights)
    {
        if (lightIndex < 16)
        {
            passCb.Lights[lightIndex++] = light->GetLightData();
        }
    }

    for (auto* light : spotLights)
    {
        if (lightIndex < 16)
        {
            passCb.Lights[lightIndex++] = light->GetLightData();
        }
    }
}