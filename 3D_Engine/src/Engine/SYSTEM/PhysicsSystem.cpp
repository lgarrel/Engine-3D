#include "pch.h"
#include "PhysicsSystem.h"

void PhysicsSystem::Update(Scene* scene, float deltaTime)
{
    if (!scene) 
        return;

    std::vector<Entity*> rigidBodies = scene->GetEntitiesWithComponent<RigidBodyComponent>();

    for (Entity* entity : rigidBodies)
    {
        if (entity->HasComponent<TransformComponent>())
        {
            auto transform = entity->GetComponent<TransformComponent>();
            auto rb = entity->GetComponent<RigidBodyComponent>();

            if (rb->isStatic) 
                continue;

            if (rb->useGravity) {
                rb->acceleration.y -= 9.81f;
            }

            DirectX::XMFLOAT3 pos = transform->GetTransform().m_localPosition;

            rb->velocity.x += rb->acceleration.x * deltaTime;
            rb->velocity.y += rb->acceleration.y * deltaTime;
            rb->velocity.z += rb->acceleration.z * deltaTime;

            pos.x += rb->velocity.x * deltaTime;
            pos.y += rb->velocity.y * deltaTime;
            pos.z += rb->velocity.z * deltaTime;

            transform->SetLocalPosition(pos);

            rb->acceleration = { 0.0f, 0.0f, 0.0f };
        }
        else
        {
            std::cout << "Erreur: Un RigidBody n'a pas de TransformComponent!" << std::endl;
        }
    }
    std::vector<Entity*> allColliders = scene->GetEntitiesWithComponent<ColliderComponent>();
    std::vector<Entity*> validColliders;

    for (Entity* e : allColliders) {
        if (e->HasComponent<TransformComponent>())
        {
            validColliders.push_back(e);

            auto c = e->GetComponent<ColliderComponent>();
            c->isColliding = false;
            c->collidedEntities.clear();
        }
    }

    for (size_t i = 0; i < validColliders.size(); i++)
    {
        auto tA = validColliders[i]->GetComponent<TransformComponent>();
        auto cA = validColliders[i]->GetComponent<ColliderComponent>();
        auto rbA = validColliders[i]->HasComponent<RigidBodyComponent>() ? validColliders[i]->GetComponent<RigidBodyComponent>() : nullptr;



        for (size_t j = i + 1; j < validColliders.size(); j++)
        {
            auto tB = validColliders[j]->GetComponent<TransformComponent>();
            auto cB = validColliders[j]->GetComponent<ColliderComponent>();
            auto rbB = validColliders[j]->HasComponent<RigidBodyComponent>() ? validColliders[j]->GetComponent<RigidBodyComponent>() : nullptr;


            int axis = CheckCollisionAABB(tA, cA, rbA, tB, cB, rbB);

            if (axis > 0)
            {
                cA->isColliding = true;
                cB->isColliding = true;

                cA->collidedEntities.push_back(validColliders[j]);
                cB->collidedEntities.push_back(validColliders[i]);

                if (axis == 2)
                {
                    if (rbA && rbA->isStatic == false) {
                        rbA->velocity.y = 0.0f;
                    }
                    if (rbB && rbB->isStatic == false) {
                        rbB->velocity.y = 0.0f;
                    }
                }
            }
        }
    }
}

void PhysicsSystem::GetWorldAABB(TransformComponent* t, ColliderComponent* c,
    float& minX, float& maxX, float& minY, float& maxY, float& minZ, float& maxZ,
    float& centerX, float& centerY, float& centerZ)
{
    XMMATRIX world = XMLoadFloat4x4(&t->GetWorldMatrix());

    XMFLOAT3 corners[8] = {
        {c->offset.x - c->extents.x, c->offset.y - c->extents.y, c->offset.z - c->extents.z},
        {c->offset.x + c->extents.x, c->offset.y - c->extents.y, c->offset.z - c->extents.z},
        {c->offset.x - c->extents.x, c->offset.y + c->extents.y, c->offset.z - c->extents.z},
        {c->offset.x + c->extents.x, c->offset.y + c->extents.y, c->offset.z - c->extents.z},
        {c->offset.x - c->extents.x, c->offset.y - c->extents.y, c->offset.z + c->extents.z},
        {c->offset.x + c->extents.x, c->offset.y - c->extents.y, c->offset.z + c->extents.z},
        {c->offset.x - c->extents.x, c->offset.y + c->extents.y, c->offset.z + c->extents.z},
        {c->offset.x + c->extents.x, c->offset.y + c->extents.y, c->offset.z + c->extents.z}
    };

    minX = minY = minZ = 1000000.0f;
    maxX = maxY = maxZ = -1000000.0f;

    for (int i = 0; i < 8; i++) 
    {
        XMVECTOR localCorner = XMVectorSet(corners[i].x, corners[i].y, corners[i].z, 1.0f);
        XMVECTOR worldCorner = XMVector4Transform(localCorner, world);

        XMFLOAT3 wc;
        XMStoreFloat3(&wc, worldCorner);

        if (wc.x < minX) 
            minX = wc.x;
        if (wc.x > maxX) 
            maxX = wc.x;
        if (wc.y < minY) 
            minY = wc.y;
        if (wc.y > maxY) 
            maxY = wc.y;
        if (wc.z < minZ) 
            minZ = wc.z;
        if (wc.z > maxZ) 
            maxZ = wc.z;
    }

    XMVECTOR localCenter = XMVectorSet(c->offset.x, c->offset.y, c->offset.z, 1.0f);
    XMVECTOR worldCenterVec = XMVector4Transform(localCenter, world);
    XMFLOAT3 wCenter;
    XMStoreFloat3(&wCenter, worldCenterVec);

    centerX = wCenter.x;
    centerY = wCenter.y;
    centerZ = wCenter.z;
}

int PhysicsSystem::CheckCollisionAABB(
    TransformComponent* tA, ColliderComponent* cA, RigidBodyComponent* rbA,
    TransformComponent* tB, ColliderComponent* cB, RigidBodyComponent* rbB)
{
    float aMinX, aMaxX, aMinY, aMaxY, aMinZ, aMaxZ, centerAx, centerAy, centerAz;
    float bMinX, bMaxX, bMinY, bMaxY, bMinZ, bMaxZ, centerBx, centerBy, centerBz;

    GetWorldAABB(tA, cA, aMinX, aMaxX, aMinY, aMaxY, aMinZ, aMaxZ, centerAx, centerAy, centerAz);
    GetWorldAABB(tB, cB, bMinX, bMaxX, bMinY, bMaxY, bMinZ, bMaxZ, centerBx, centerBy, centerBz);

    bool collisionX = aMaxX >= bMinX && bMaxX >= aMinX;
    bool collisionY = aMaxY >= bMinY && bMaxY >= aMinY;
    bool collisionZ = aMaxZ >= bMinZ && bMaxZ >= aMinZ;

    if (collisionX && collisionY && collisionZ)
    {
        float penX = std::fmin(aMaxX - bMinX, bMaxX - aMinX);
        float penY = std::fmin(aMaxY - bMinY, bMaxY - aMinY);
        float penZ = std::fmin(aMaxZ - bMinZ, bMaxZ - aMinZ);

        int collisionAxis = 0;
        float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f;

        if (penY <= penX && penY <= penZ) 
        {
            if (centerAy > centerBy)
                moveY = penY;
            else
                moveY =  -penY;

            collisionAxis = 2;
        }
        else if (penX <= penY && penX <= penZ) 
        {
            if (centerAx > centerBx)
                moveX = penX;
            else
                moveX = -penX;

            collisionAxis = 1;
        }
        else 
        {
            if (centerAz > centerBz)
                moveZ = penZ;
            else
                moveZ = -penZ;
            

            collisionAxis = 3;
        }

        bool aIsStatic = (rbA != nullptr && rbA->isStatic) || (rbA == nullptr);
        bool bIsStatic = (rbB != nullptr && rbB->isStatic) || (rbB == nullptr);

        if (aIsStatic == false && bIsStatic)
        {
            DirectX::XMFLOAT3 pos = tA->GetTransform().m_localPosition;
            pos.x += moveX; 
            pos.y += moveY; 
            pos.z += moveZ;
            tA->SetLocalPosition(pos);
        }
        else if (aIsStatic && bIsStatic == false)
        {
            DirectX::XMFLOAT3 pos = tB->GetTransform().m_localPosition;
            pos.x -= moveX; 
            pos.y -= moveY; 
            pos.z -= moveZ;
            tB->SetLocalPosition(pos);
        }
        else if (aIsStatic == false && bIsStatic == false)
        {
            DirectX::XMFLOAT3 posA = tA->GetTransform().m_localPosition;
            posA.x += moveX * 0.5f; 
            posA.y += moveY * 0.5f; 
            posA.z += moveZ * 0.5f;
            tA->SetLocalPosition(posA);

            DirectX::XMFLOAT3 posB = tB->GetTransform().m_localPosition;
            posB.x -= moveX * 0.5f; 
            posB.y -= moveY * 0.5f; 
            posB.z -= moveZ * 0.5f;
            tB->SetLocalPosition(posB);
        }

        return collisionAxis;
    }

    return 0;
}