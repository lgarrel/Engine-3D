#pragma once

class GameOverScene : public Scene
{
private:
    Entity* m_uiGameOverScreen;
    Entity* m_uiFinalScore;

    Device* m_device;
    GraphicsEngine* m_graphicsEngine;

    int m_finalScore;

public:
    GameOverScene();
    ~GameOverScene();

    void SetDevice(Device* device);
    void SetGraphicsEngine(GraphicsEngine* engine);
    void SetFinalScore(int score);

    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;
};