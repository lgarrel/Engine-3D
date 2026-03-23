#pragma once
#include "Component.h"

class Entity;

class PlayerControllerComponent : public Component
{
public:
    float m_moveSpeed;
    float m_rotationSpeed;

    Entity* m_movementTarget;
    Entity* m_rotationTarget;

    float m_currentPitch;
    float m_minPitch;
    float m_maxPitch;

    PlayerControllerComponent();

    const char* GetTypeName() const override;
};