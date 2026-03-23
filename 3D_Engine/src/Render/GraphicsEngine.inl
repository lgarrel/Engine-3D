#include "../Engine/SceneManager.h"
#include "RenderSystem.h"
#include "SwapChain.h"
#include "RenderUI.h"

template <typename Scene>
void GraphicsEngine::Initialize(HWND hwnd, int width, int height)
{
    m_width = width;
    m_height = height;

    m_device = std::make_unique<Device>();

    m_swapChain = std::make_unique<SwapChain>();
    m_swapChain->Initialize(m_device.get(), hwnd, width, height);

    m_screenViewport.TopLeftX = 0;
    m_screenViewport.TopLeftY = 0;
    m_screenViewport.Width = static_cast<float>(width);
    m_screenViewport.Height = static_cast<float>(height);
    m_screenViewport.MinDepth = 0.0f;
    m_screenViewport.MaxDepth = 1.0f;

    m_scissorRect = { 0, 0, width, height };

    m_mainContext = std::make_unique<CommandContext>();
    m_mainContext->Initialize(m_device.get());

    BuildConstantBuffer();
    BuildDescriptorHeap();
    BuildUIMesh();
    BuildPSO();

    SceneManager* sm = SceneManager::GetInstance();
    Scene* testScene = sm->Add<Scene>();
    testScene->SetDevice(m_device.get());
    testScene->SetGraphicsEngine(this);
    


    sm->SetCurrentScene<Scene>();
    m_currentScene = sm->GetCurrentScene();

    m_cameraEntity = static_cast<Scene*>(m_currentScene)->GetCamera();

    m_cameraSystem = std::make_unique<CameraSystem>();

    m_renderSystem = std::make_unique<RenderSystem>();

    m_lightSystem = std::make_unique<LightSystem>();
    m_renderSystem->Initialize(m_mainContext.get(), m_device.get(), m_objectCB.get(), m_passCB.get(), m_cbvHeap.Get(), m_lightSystem.get());

    m_renderUI = std::make_unique<RenderUI>();
    m_renderUI->Initialize(m_mainContext.get(), m_device.get(), m_uiCB.get(), m_cbvHeap.Get(), m_uiPso.Get(), m_uiMesh.get());



    m_transformSystem = std::make_unique<TransformSystem>();

    m_playerController = std::make_unique<PlayerControllerSystem>();

    m_physicsSystem = std::make_unique<PhysicsSystem>();

    XMMATRIX orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    XMStoreFloat4x4(&m_orthoProj, orthoMatrix);
}
