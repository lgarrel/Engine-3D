#include "pch.h"
#include "CameraSystem.h"

CameraSystem::CameraSystem()
{
}

void CameraSystem::Update(Scene* currentScene, float deltaTime)
{
    std::vector<Entity*> cameraEntities = currentScene->GetEntitiesWithComponent<CameraComponent>();

    for (Entity* entity : cameraEntities)
    {
        if (entity == nullptr)
            continue;

        TransformComponent* tc = entity->GetComponent<TransformComponent>();
        CameraComponent* camComp = entity->GetComponent<CameraComponent>();

        if (camComp == nullptr || tc == nullptr)
            continue;

        if (entity->HasParent())
        {
            TransformComponent* parentTc = entity->GetParent()->GetComponent<TransformComponent>();
            if (parentTc != nullptr)
            {
                Transform& t = tc->GetTransform();
                XMFLOAT3 camPos = t.m_worldPosition;
                XMFLOAT3 targetPos = parentTc->GetTransform().m_worldPosition;
                XMFLOAT3 up = { 0.f, 1.f, 0.f };

                XMVECTOR dir = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetPos), XMLoadFloat3(&camPos)));
                XMFLOAT3 dirF;
                XMStoreFloat3(&dirF, dir);

                t.LookTo(dirF.x, dirF.y, dirF.z, up);
            }
        }

        camComp->GetCamera().OnUpdate(&tc->GetTransform());
    }
}
bool CameraSystem::GetMainCameraMatrices(Scene* currentScene, XMFLOAT4X4& outView, XMFLOAT4X4& outProj)
{
	std::vector<Entity*> cameraEntities = currentScene->GetEntitiesWithComponent<CameraComponent>();

	for (Entity* entity : cameraEntities)
	{
		if (entity == nullptr)
			continue;

		CameraComponent* cameraComp = entity->GetComponent<CameraComponent>();
		if (cameraComp == nullptr)
			continue;

		outView = cameraComp->GetView();
		outProj = cameraComp->GetProj();
		return true;
	}

	return false;
}