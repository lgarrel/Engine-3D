#include "pch.h"
#include "PlayerControllerSystem.h"

PlayerControllerSystem::PlayerControllerSystem()
{
}

void PlayerControllerSystem::Update(Scene* scene, float deltaTime)
{
    if (!scene) 
        return;


    std::vector<Entity*> players = scene->GetEntitiesWithComponent<PlayerControllerComponent>();

    for (Entity* entity : players)
    {
        auto playerComp = entity->GetComponent<PlayerControllerComponent>();

        Entity* moveEntity = nullptr;
        if (playerComp->m_movementTarget != nullptr)
        {
            moveEntity = playerComp->m_movementTarget;
        }
        else
        {
            moveEntity = entity;
        }

        Entity* rotEntity = nullptr;
        if (playerComp->m_rotationTarget != nullptr)
        {
            rotEntity = playerComp->m_rotationTarget;
        }
        else
        {
            rotEntity = entity;
        }

        InputManager* input = InputManager::GetInstance();
         
        if (rotEntity->HasComponent<TransformComponent>())
        {
            auto rotTransformComp = rotEntity->GetComponent<TransformComponent>();
            const float mouseSensitivity = 0.002f;
            int deltaX = input->GetMouseDeltaX();
            int deltaY = input->GetMouseDeltaY();
             
            if (deltaX != 0)
            {
                float angleY = static_cast<float>(deltaX) * mouseSensitivity;
                rotTransformComp->RotateLocalY(angleY * deltaTime * playerComp->m_rotationSpeed * 50.f);
            }
             
            if (deltaY != 0)
            {
                float angleX = static_cast<float>(deltaY) * mouseSensitivity * deltaTime * playerComp->m_rotationSpeed * 50.f;
                float targetPitch = playerComp->m_currentPitch + angleX;

                if (targetPitch > playerComp->m_maxPitch)
                {
                    targetPitch = playerComp->m_maxPitch;
                }

                if (targetPitch < playerComp->m_minPitch)
                {
                    targetPitch = playerComp->m_minPitch;
                }

                float actualDeltaX = targetPitch - playerComp->m_currentPitch;
                playerComp->m_currentPitch = targetPitch;

                rotTransformComp->RotateLocalX(actualDeltaX);
            }

            if (input->IsKeyDown('Q') || input->IsKeyDown(VK_LEFT) || input->GetRightStickX() < -0.3f)
            {
                rotTransformComp->RotateLocalY(-playerComp->m_rotationSpeed * deltaTime);
            }

            if (input->IsKeyDown('D') || input->IsKeyDown(VK_RIGHT) || input->GetRightStickX() > 0.3f)
            {
                rotTransformComp->RotateLocalY(playerComp->m_rotationSpeed * deltaTime);
            }
        }
         
        if (moveEntity->HasComponent<TransformComponent>() && moveEntity->HasComponent<RigidBodyComponent>())
        {
            auto moveTransformComp = moveEntity->GetComponent<TransformComponent>();
            auto rb = moveEntity->GetComponent<RigidBodyComponent>();

            XMFLOAT3 forward = moveTransformComp->GetTransform().m_forward;
            forward.y = 0.0f;

            rb->velocity.x = 0.0f;
            rb->velocity.z = 0.0f;


            if (input->IsKeyDown('Z') || input->IsKeyDown(VK_UP) || input->GetLeftStickY() > 0.3f )
            {
                rb->velocity.x += forward.x * playerComp->m_moveSpeed;
                rb->velocity.z += forward.z * playerComp->m_moveSpeed;
            }
            if (input->IsKeyDown('S') || input->IsKeyDown(VK_DOWN) || input->GetLeftStickY() < -0.3f)
            {
                rb->velocity.x -= forward.x * playerComp->m_moveSpeed;
                rb->velocity.z -= forward.z * playerComp->m_moveSpeed;
            }
        }
    }
}