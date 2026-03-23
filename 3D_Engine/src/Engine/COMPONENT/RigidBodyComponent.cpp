#include "pch.h"
#include "RigidBodyComponent.h"


RigidBodyComponent::RigidBodyComponent()
{
    velocity = { 0.0f, 0.0f, 0.0f };
    acceleration = { 0.0f, 0.0f, 0.0f };
    useGravity = true;
    isStatic = true;
    mass = 1.0f;
}


void RigidBodyComponent::AddForce(const DirectX::XMFLOAT3& force)
{
    if (isStatic) 
        return;

    acceleration.x += force.x / mass;
    acceleration.y += force.y / mass;
    acceleration.z += force.z / mass;
}