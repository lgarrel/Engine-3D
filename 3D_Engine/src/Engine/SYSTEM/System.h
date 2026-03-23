#pragma once

class Scene;

class System
{
public:
    virtual ~System() = default;
    virtual void Update(Scene* scene, float deltaTime) = 0;
};