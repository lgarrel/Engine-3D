#pragma once

class PlayerControllerSystem : public System
{
public:

    PlayerControllerSystem();

    virtual void Update(Scene* currentScene, float deltaTime) override;

};

