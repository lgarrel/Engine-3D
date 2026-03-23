#pragma once

class RigidBodyComponent : public Component
{
public:

    RigidBodyComponent();

    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;
    bool useGravity;
    bool isStatic;
    float mass;


    void AddForce(const DirectX::XMFLOAT3& force);

    const char* GetTypeName() const override { return "RigidBodyComponent"; }
};