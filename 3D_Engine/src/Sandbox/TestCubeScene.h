#pragma once

class GraphicsEngine;
class S_Player;

class TestCubeScene : public Scene
{
private:
    S_Player* m_player = nullptr;
    Entity* m_cameraEntity = nullptr;
    Device* mp_device = nullptr; 
    GraphicsEngine* m_graphicsEngine = nullptr;

    Vector<S_Rail*> mv_rails;

    int m_railNB = 10;//past 30 rails don't load completly

public:
    TestCubeScene() = default;

    void SetDevice(Device* device);

    void SetGraphicsEngine(GraphicsEngine* engine);

    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;

    void DeleteRail();

    Entity* GetCamera() const { return m_cameraEntity; }
};

