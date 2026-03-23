#include "pch.h"
#include "TestGameplay.h"
#include "Enemy.h"
#include "GameOverScene.h"
#include "SceneManager.h"

TestGameplay::TestGameplay()
{
}

void TestGameplay::SetDevice(Device* device)
{
    mp_device = device;
}

void TestGameplay::SetGraphicsEngine(GraphicsEngine* engine)
{
    m_graphicsEngine = engine;
}

void TestGameplay::Init()
{
    int indexBasicTexture = m_graphicsEngine->Load3DTexture(L"../../res/Texture/White_Basic.png");
    int indexTextureCaisse = m_graphicsEngine->Load3DTexture(L"../../res/Texture/caisseTexture2.png");

    m_player = new Player(this, mp_device, indexTextureCaisse);

    mp_manager = new RailChunkManager();
    mp_manager->Init(mp_device, this, m_player->GetWagon());

    Entity* wagon = m_player->GetWagon();

    m_cameraEntity = CreateEntity();
    TransformComponent* camTransform = m_cameraEntity->AddComponent<TransformComponent>();
    camTransform->SetLocalPosition(XMFLOAT3(0.0f, 3.0f, 0.0f));

    CameraComponent* camComp = m_cameraEntity->AddComponent<CameraComponent>();
    camComp->SetPerspective(XM_PIDIV4, 1280.0f / 720.0f, 0.1f, 1000.0f);

    m_cameraEntity->AddComponent<PlayerControllerComponent>();

    m_uiScoreText = CreateEntity();
    UIRendererComponent* uiTextComp = m_uiScoreText->AddComponent<UIRendererComponent>();
    uiTextComp->position = XMFLOAT2(50.f, 50.f);
    uiTextComp->size = XMFLOAT2(32.f, 32.f);
    uiTextComp->text = "Score : 0";
    uiTextComp->color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

    int indexTextureHeart = m_graphicsEngine->Load3DTexture(L"../../res/Texture/Heart.png");
    int indexTextureCrosshair = m_graphicsEngine->Load3DTexture(L"../../res/Texture/crosshair.png");

    for (int i = 0; i < 3; i++)
    {
        Entity* uiHeartEntity = CreateEntity();
        UIRendererComponent* uiHeartComp = uiHeartEntity->AddComponent<UIRendererComponent>();
        uiHeartComp->position = XMFLOAT2(1100.f + 40 * i, 50.f);
        uiHeartComp->size = XMFLOAT2(50.f, 32.f);
        uiHeartComp->text = "";
        uiHeartComp->textureIndex = indexTextureHeart;
        uiHeartComp->color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

        mv_uiHearts.push_back(uiHeartEntity);
    }

    Entity* uiCrosshairEntity = CreateEntity();
    UIRendererComponent* uiCrosshairComp = uiCrosshairEntity->AddComponent<UIRendererComponent>();
    uiCrosshairComp->position = XMFLOAT2(WIN_WIDTH * 0.5f - 25.f, WIN_HEIGHT * 0.5f - 25.f);
    uiCrosshairComp->size = XMFLOAT2(50.f, 50.f);
    uiCrosshairComp->text = "";
    uiCrosshairComp->textureIndex = indexTextureCrosshair;
    uiCrosshairComp->color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

    Entity* lightEntity = CreateEntity();
    LightComponent* lightComp = lightEntity->AddComponent<LightComponent>();
    lightComp->SetType(LightComponent::LightType::Directional);
    lightComp->SetStrength(XMFLOAT3(1.0f, 1.0f, 1.0f));
    lightComp->SetDirection({ 0.f, 0.f, 1.f });

    Entity* lightEntity2 = CreateEntity();
    LightComponent* lightComp2 = lightEntity2->AddComponent<LightComponent>();
    lightComp2->SetType(LightComponent::LightType::Directional);
    lightComp2->SetStrength(XMFLOAT3(1.0f, 1.0f, 1.0f));


    //SET CHILD
    //m_player->GetTurrel()->AddChild(m_cameraEntity);
    m_cameraEntity->AddChild(m_player->GetCannon());
    //m_cameraEntity->AddChild(m_player->GetTurrel());
    //m_cameraEntity->AddChild(m_player->GetWagon());
    //wagon->AddChild(refCubeEntity);
}

void TestGameplay::Update(float deltaTime)
{
    if (m_player != nullptr)
    {
        int oldHp = m_player->GetHP();
        m_player->Update(deltaTime);

        if (m_player->GetHP() != oldHp)
        {
            UpdateHealthUI();
        }

        if (m_player->GetHP() <= 0)
        {
            SceneManager* manager = SceneManager::GetInstance();
            GameOverScene* gameOverScene = manager->Get<GameOverScene>();

            if (gameOverScene == nullptr)
            {
                gameOverScene = manager->Add<GameOverScene>();

                gameOverScene->SetDevice(mp_device);
                gameOverScene->SetGraphicsEngine(m_graphicsEngine);
            }

            gameOverScene->SetFinalScore(m_score);

            manager->SetCurrentScene<GameOverScene>();
        }
    }

    if (m_enemy != nullptr)
    {
        m_enemy->Update(deltaTime);

        if (m_enemy->IsDead())
        {
            m_score = m_score + 10;
            UpdateScoreUI();

            delete m_enemy;
            m_enemy = nullptr;
        }
    }

    Scene::Update(deltaTime);

    if (mp_manager) {
        mp_manager->Update(deltaTime);
    }
}

void TestGameplay::UpdateScoreUI()
{
    if (m_uiScoreText != nullptr)
    {
        UIRendererComponent* uiComp = m_uiScoreText->GetComponent<UIRendererComponent>();
        if (uiComp != nullptr)
        {
            uiComp->text = "Score : " + std::to_string(m_score);
        }
    }
}

void TestGameplay::UpdateHealthUI()
{
    int hp = 0;
    if (m_player != nullptr)
    {
        hp = m_player->GetHP();
    }

    for (int i = 0; i < mv_uiHearts.size(); i++)
    {
        UIRendererComponent* heartComp = mv_uiHearts[i]->GetComponent<UIRendererComponent>();
        if (heartComp != nullptr)
        {
            if (i < hp)
            {
                heartComp->color.w = 1.0f;
            }
            else
            {
                heartComp->color.w = 0.0f;
            }
        }
    }
}

void TestGameplay::Render()
{
}

Entity* TestGameplay::GetCamera()
{
    return m_cameraEntity;
}

