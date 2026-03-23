#pragma once

#include "../Engine/SYSTEM/System.h"

class Scene;
class CommandContext;
class Device;
class Mesh;
struct UIConstants;
template<typename T> class UploadBuffer;
struct ID3D12DescriptorHeap;
struct ID3D12PipelineState;

class RenderUI : public System
{
public:
    RenderUI();

    void Initialize(CommandContext* context, Device* device, UploadBuffer<UIConstants>* uiCB,
        ID3D12DescriptorHeap* cbvHeap, ID3D12PipelineState* uiPso, Mesh* uiMesh);

    void Update(Scene* currentScene, float deltaTime) override;

    void Render(Scene* scene, const XMFLOAT4X4& orthoProj);

private:
    CommandContext* m_context = nullptr;
    Device* m_device = nullptr;
    UploadBuffer<UIConstants>* m_uiCB = nullptr;
    ID3D12DescriptorHeap* m_cbvHeap = nullptr;
    ID3D12PipelineState* m_uiPso = nullptr;
    Mesh* m_uiMesh = nullptr;
};
