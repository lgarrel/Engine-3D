#pragma once

#include "../Engine/SYSTEM/System.h"  
#include "Light.h"                    
#include "MaterialComponent.h"         

class Scene;
class Entity;
class PlayerControllerSystem;
class Mesh;
class PSOUtils;
class Device;
class SwapChain;
class CommandContext;
class CameraSystem;
class LightSystem;
class TransformSystem;
class RenderSystem;
class PhysicsSystem;
class RenderUI;


template<typename T> class UploadBuffer;

template<typename T> class UploadBuffer;

struct ObjectConstants
{
    XMFLOAT4X4 m_world;
    Material m_material;
    XMFLOAT4 Color;
    XMFLOAT4 UvRect;
    ObjectConstants();
};

struct UIConstants
{
    XMFLOAT4X4 m_world;
    XMFLOAT4X4 m_view;
    XMFLOAT4X4 m_proj;
    XMFLOAT4 Color;
    XMFLOAT4 UvRect;
    UIConstants();
};

struct PassConstants
{
    XMFLOAT4X4 View;
    XMFLOAT4X4 Proj;
    XMFLOAT3 EyePosW;
    float cbPad1;
    Light Lights[16];
};

class GraphicsEngine : public System
{
public:
    template <typename Scene>
    void Initialize(HWND hwnd, int width, int height);

    void Update(Scene* scene, float deltaTime) override;
    void Update(float deltaTime);

    void Render();

    void OnResize(int width, int height);

    void LoadUITexture(const std::wstring& filepath);
    int Load3DTexture(const std::wstring& filepath);

    ~GraphicsEngine();

    Entity* GetCameraEntity();

    static const int m_maxObjects = 500;
    static const int m_maxPass = 1;
    static const int m_maxTextures = 50;

private:
    void BuildConstantBuffer();
    void BuildDescriptorHeap();
    void BuildPSO();
    void BuildUIMesh();
    void UpdateSystem(Scene* scene, float deltaTime);

    std::unique_ptr<Device> m_device;
    std::unique_ptr<SwapChain> m_swapChain;
    std::unique_ptr<CommandContext> m_mainContext;

    ComPtr<ID3D12PipelineState> m_pso;
    ComPtr<ID3D12PipelineState> m_uiPso;

    std::unique_ptr<UploadBuffer<ObjectConstants>> m_objectCB;
    std::unique_ptr<UploadBuffer<UIConstants>> m_uiCB;
    std::unique_ptr<UploadBuffer<PassConstants>> m_passCB;

    ComPtr<ID3D12DescriptorHeap> m_cbvHeap;

    D3D12_VIEWPORT m_screenViewport;
    D3D12_RECT m_scissorRect;

    Scene* m_currentScene = nullptr;
    Entity* m_cameraEntity = nullptr;

    int m_width = 0;
    int m_height = 0;

    std::unique_ptr<CameraSystem> m_cameraSystem;
    std::unique_ptr<RenderSystem> m_renderSystem;
    std::unique_ptr<RenderUI> m_renderUI;
    std::unique_ptr<TransformSystem> m_transformSystem;
    std::unique_ptr<LightSystem> m_lightSystem;
    std::unique_ptr<PlayerControllerSystem> m_playerController;
    std::unique_ptr<PhysicsSystem> m_physicsSystem;

    XMFLOAT4X4 m_orthoProj;

    std::unique_ptr<Mesh> m_uiMesh;

    ComPtr<ID3D12Resource> m_uiTexture;
    ComPtr<ID3D12Resource> m_uiTextureUpload;

    std::vector<ComPtr<ID3D12Resource>> m_3dTextures;
    std::vector<ComPtr<ID3D12Resource>> m_3dTextureUploads;
    int m_currentTextureCount = 0;
};

#include "GraphicsEngine.inl"