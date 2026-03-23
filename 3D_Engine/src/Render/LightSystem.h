#pragma once
class PassConstants;

class LightSystem : public System
{
public:
    LightSystem() = default;
    virtual ~LightSystem() = default;

    void Update(Scene* scene, float deltaTime) override;

    void FillPassConstants(Scene* scene, PassConstants& passCb);
};

