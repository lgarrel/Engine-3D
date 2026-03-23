#pragma once

class TestSceneRail : public Scene
{
private:
    Vector<S_Rail*> mv_rails;
    Entity* mp_cameraEntity = nullptr;

    S_Rail* mp_currentRail = nullptr;//mv_rails[0] et plus de varMembre

    int m_railNB = 10;//past 30 rails don't load completly

    static TestSceneRail* mp_instance;

public:
    static TestSceneRail* GetInstance();

    TestSceneRail() = default;
    Device* m_device = nullptr;

    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;

    void DeleteRail();

    Entity* GetCamera() const { return mp_cameraEntity; }
};

