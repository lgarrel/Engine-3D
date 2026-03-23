#include "pch.h"
#include "StartScene.h"

StartScene::StartScene()
{
    m_uiStartScreen = nullptr;
    m_cameraEntity = nullptr;
    m_device = nullptr;
    m_graphicsEngine = nullptr;
}

StartScene::~StartScene()
{
}

void StartScene::SetDevice(Device* device)
{
    m_device = device;
}

void StartScene::SetGraphicsEngine(GraphicsEngine* engine)
{
    m_graphicsEngine = engine;
}

void StartScene::Init()
{ 
    m_cameraEntity = CreateEntity();
    TransformComponent* camTransform = m_cameraEntity->AddComponent<TransformComponent>();
    camTransform->SetLocalPosition(XMFLOAT3(0.0f, 3.0f, -10.0f));

    CameraComponent* camComp = m_cameraEntity->AddComponent<CameraComponent>();
    camComp->SetPerspective(XM_PIDIV4, 1280.0f / 720.0f, 0.1f, 1000.0f);
     
    m_uiStartScreen = CreateEntity();
    UIRendererComponent* startComp = m_uiStartScreen->AddComponent<UIRendererComponent>();

    startComp->position = XMFLOAT2(400.f, 400.f);
    startComp->size = XMFLOAT2(32.f, 32.f);
    startComp->text = "Press Space To Play";
    startComp->color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
}

void StartScene::Update(float deltaTime)
{
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        SceneManager* manager = SceneManager::GetInstance();

        TestGameplay* gameplayScene = manager->Get<TestGameplay>();

        if (gameplayScene == nullptr)
        {
            gameplayScene = manager->Add<TestGameplay>();

            gameplayScene->SetDevice(m_device);
            gameplayScene->SetGraphicsEngine(m_graphicsEngine);
        }

        manager->SetCurrentScene<TestGameplay>();
    }

    Scene::Update(deltaTime);
}

void StartScene::Render()
{
}

Entity* StartScene::GetCamera() const
{
    return m_cameraEntity;
}