#include "pch.h"
#include "PlayerControllerComponent.h"

PlayerControllerComponent::PlayerControllerComponent() : m_moveSpeed(30.0f), m_rotationSpeed(1.0f), m_movementTarget(nullptr), m_rotationTarget(nullptr), m_currentPitch(0.0f), m_minPitch(-0.5f), m_maxPitch(0.2f)
{
}

const char* PlayerControllerComponent::GetTypeName() const
{
    return "PlayerControllerComponent";
}