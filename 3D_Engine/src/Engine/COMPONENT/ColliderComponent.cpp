#include "pch.h"
#include "ColliderComponent.h"

ColliderComponent::ColliderComponent()
{
    extents = { 0.5f, 0.5f, 0.5f };

    offset = { 0.0f, 0.0f, 0.0f };

    isColliding = false;
}