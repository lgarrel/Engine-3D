#pragma once

#include "Scene.h"


enum class GameState
{
    Splash,
    Start,
    Playing,
    GameOver
};

class TestGameplay : public Scene
{
private:
    Player* m_player = nullptr;
    Enemy* m_enemy = nullptr;
    Entity* m_cameraEntity = nullptr;
    Device* mp_device = nullptr;

    GraphicsEngine* m_graphicsEngine = nullptr;

    Vector<Rail*> mv_rails;

    int m_railNB = 10;
    int m_score = 0;

    RailChunkManager* mp_manager = nullptr;

    Entity* m_uiScoreText = nullptr;
    std::vector<Entity*> mv_uiHearts;

    GameState m_currentState;
    float m_splashTimer;

    Entity* m_uiSplashScreen = nullptr;
    Entity* m_uiStartScreen = nullptr;
    Entity* m_uiGameOverScreen = nullptr;
    Entity* m_uiFinalScoreText = nullptr;

public:
    TestGameplay(); 

    void SetDevice(Device* device);
    void SetGraphicsEngine(GraphicsEngine* engine);

    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;

    void DeleteRail();

    void CreateRail();

    void UpdateScoreUI();
    void UpdateHealthUI();

    Entity* GetCamera();

    void ChangeState(GameState newState);
    void ShowUIForCurrentState();
    void TriggerGameOver();
};