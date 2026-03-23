#pragma once  

class Scene;
class TransformComponent;
class ColliderComponent;
class RigidBodyComponent;

class PhysicsSystem : public System
{
public:

    void Update(Scene* scene, float deltaTime);

private:

    int CheckCollisionAABB(
        TransformComponent* tA, ColliderComponent* cA, RigidBodyComponent* rbA,
        TransformComponent* tB, ColliderComponent* cB, RigidBodyComponent* rbB);

    void GetWorldAABB(TransformComponent* t, ColliderComponent* c,
        float& minX, float& maxX, float& minY, float& maxY, float& minZ, float& maxZ,
        float& centerX, float& centerY, float& centerZ);
};
