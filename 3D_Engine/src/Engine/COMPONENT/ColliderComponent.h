#pragma once  

class Entity;

class ColliderComponent : public Component
{
public:

    ColliderComponent();

    DirectX::XMFLOAT3 extents;
    DirectX::XMFLOAT3 offset;
    bool isColliding;
    
    std::vector<Entity*> collidedEntities;

    const char* GetTypeName() const override { return "ColliderComponent"; }
};
