#include "pch.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene()
{
    m_uiGameOverScreen = nullptr;
    m_uiFinalScore = nullptr;
    m_device = nullptr;
    m_graphicsEngine = nullptr;
    m_finalScore = 0;
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::SetDevice(Device* device)
{
    m_device = device;
}

void GameOverScene::SetGraphicsEngine(GraphicsEngine* engine)
{
    m_graphicsEngine = engine;
}

void GameOverScene::SetFinalScore(int score)
{
    m_finalScore = score;
}

void GameOverScene::Init()
{
    m_uiGameOverScreen = CreateEntity();
    UIRendererComponent* gameOverComp = m_uiGameOverScreen->AddComponent<UIRendererComponent>();
    gameOverComp->position = XMFLOAT2(400.f, 200.f);
    gameOverComp->size = XMFLOAT2(64.f, 64.f);
    gameOverComp->text = "DEFAITE";
    gameOverComp->color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

    m_uiFinalScore = CreateEntity();
    UIRendererComponent* finalScoreComp = m_uiFinalScore->AddComponent<UIRendererComponent>();
    finalScoreComp->position = XMFLOAT2(400.f, 400.f);
    finalScoreComp->size = XMFLOAT2(32.f, 32.f);
    finalScoreComp->text = "Score : " + std::to_string(m_finalScore);
    finalScoreComp->color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
}

void GameOverScene::Update(float deltaTime)
{
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        SceneManager* manager = SceneManager::GetInstance();
        TestGameplay* startScene = manager->Get<TestGameplay>();

        manager->DeleteScene<TestGameplay>();

        if (startScene == nullptr)
        {
            startScene = manager->Add<TestGameplay>();

            startScene->SetDevice(m_device);
            startScene->SetGraphicsEngine(m_graphicsEngine);
        }


        manager->SetCurrentScene<TestGameplay>();
    }

    Scene::Update(deltaTime);
}

void GameOverScene::Render()
{
}