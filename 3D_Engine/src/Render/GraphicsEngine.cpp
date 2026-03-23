#include "pch.h"
#include "GraphicsEngine.h"

#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

ObjectConstants::ObjectConstants()
{
    Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    UvRect = { 0.0f, 0.0f, 1.0f, 1.0f };
}

UIConstants::UIConstants()
{
    XMStoreFloat4x4(&m_world, XMMatrixIdentity());
    XMStoreFloat4x4(&m_view, XMMatrixIdentity());
    XMStoreFloat4x4(&m_proj, XMMatrixIdentity());
    Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    UvRect = { 0.0f, 0.0f, 1.0f, 1.0f };
}

void GraphicsEngine::BuildConstantBuffer()
{
    m_objectCB = std::make_unique<UploadBuffer<ObjectConstants>>(m_device->GetNativeDevice(), m_maxObjects, true);
    m_uiCB = std::make_unique<UploadBuffer<UIConstants>>(m_device->GetNativeDevice(), m_maxObjects, true);
    m_passCB = std::make_unique<UploadBuffer<PassConstants>>(m_device->GetNativeDevice(), m_maxPass, true);
}

void GraphicsEngine::BuildDescriptorHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = m_maxObjects + m_maxObjects + m_maxPass + 1 + m_maxTextures;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    m_device->GetNativeDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvHeap));

    UINT objCbvSize = UploadBuffer<ObjectConstants>::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    UINT uiCbvSize = UploadBuffer<UIConstants>::CalcConstantBufferByteSize(sizeof(UIConstants));
    UINT passCbvSize = UploadBuffer<PassConstants>::CalcConstantBufferByteSize(sizeof(PassConstants));

    for (int i = 0; i < m_maxObjects; i++)
    {
        // Object Constants
        D3D12_GPU_VIRTUAL_ADDRESS addr = m_objectCB->Resource()->GetGPUVirtualAddress() + (UINT64)i * objCbvSize;

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = addr;
        cbvDesc.SizeInBytes = objCbvSize;

        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += (SIZE_T)i * m_device->GetCbvSrvUavSize();

        m_device->GetNativeDevice()->CreateConstantBufferView(&cbvDesc, handle);

        // UI Constants
        D3D12_GPU_VIRTUAL_ADDRESS addrUI = m_uiCB->Resource()->GetGPUVirtualAddress() + (UINT64)i * uiCbvSize;

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDescUI = {};
        cbvDescUI.BufferLocation = addrUI;
        cbvDescUI.SizeInBytes = uiCbvSize;

        D3D12_CPU_DESCRIPTOR_HANDLE handleUI = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
        handleUI.ptr += (SIZE_T)(m_maxObjects + i) * m_device->GetCbvSrvUavSize();

        m_device->GetNativeDevice()->CreateConstantBufferView(&cbvDescUI, handleUI);
    }

    for (int i = 0; i < m_maxPass; i++)
    {
        D3D12_GPU_VIRTUAL_ADDRESS passAddr = m_passCB->Resource()->GetGPUVirtualAddress() + (UINT64)i * passCbvSize;

        D3D12_CONSTANT_BUFFER_VIEW_DESC passCbvDesc = {};
        passCbvDesc.BufferLocation = passAddr;
        passCbvDesc.SizeInBytes = passCbvSize;

        D3D12_CPU_DESCRIPTOR_HANDLE passHandle = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
        passHandle.ptr += (SIZE_T)(2 * m_maxObjects + i) * m_device->GetCbvSrvUavSize();

        m_device->GetNativeDevice()->CreateConstantBufferView(&passCbvDesc, passHandle);
    }
}

void GraphicsEngine::BuildPSO()
{
    ShaderUtils su;
    su.BuildShader();

    if (!su.GetMVSCode() || !su.GetMPSCode() || !su.GetMVSUICode() || !su.GetMPSUICode()) {
        throw std::runtime_error("Erreur critique : Impossible de compiler les shaders.");
    }

    PSOUtils psoUtils;
    m_pso = psoUtils.BuildPSO(m_device.get(), &su);
    m_uiPso = psoUtils.BuildUIPSO(m_device.get(), &su);
}

void GraphicsEngine::Update(Scene* scene, float deltaTime)
{
    UpdateSystem(scene, deltaTime);

    if (scene)
        scene->Update(deltaTime);
}

void GraphicsEngine::Update(float deltaTime)
{
    m_currentScene = SceneManager::GetInstance()->GetCurrentScene();

    if (m_currentScene != nullptr)
    {
        m_cameraEntity = m_currentScene->GetCamera();
    }

    Update(m_currentScene, deltaTime);
}

void GraphicsEngine::Render()
{
    m_mainContext->Reset();
    auto cmdList = m_mainContext->List();

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_swapChain->CurrentBuffer();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    cmdList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_swapChain->CurrentBufferView();
    D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_swapChain->DepthStencilView();

    const float clearColor[] = { 0.69f, 0.84f, 0.9f, 1.0f };
    cmdList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    cmdList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    cmdList->OMSetRenderTargets(1, &rtv, FALSE, &dsv);

    cmdList->SetPipelineState(m_pso.Get());
    cmdList->RSSetViewports(1, &m_screenViewport);
    cmdList->RSSetScissorRects(1, &m_scissorRect);
    cmdList->SetGraphicsRootSignature(m_device->GetRootSignature());

    ID3D12DescriptorHeap* heaps[] = { m_cbvHeap.Get() };
    cmdList->SetDescriptorHeaps(1, heaps);

    m_renderSystem->RenderScene(m_currentScene, m_cameraSystem.get());
    m_renderUI->Render(m_currentScene, m_orthoProj);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    cmdList->ResourceBarrier(1, &barrier);

    m_mainContext->Execute(true);
    m_swapChain->Present();
}

void GraphicsEngine::LoadUITexture(const std::wstring& filepath)
{
    HRESULT hr;
    ComPtr<IWICImagingFactory> wicFactory;
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

    ComPtr<IWICBitmapDecoder> decoder;
    hr = wicFactory->CreateDecoderFromFilename(filepath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

    UINT width = 1, height = 1;
    std::vector<BYTE> pixels(4, 255);

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICBitmapFrameDecode> frame;
        decoder->GetFrame(0, &frame);
        frame->GetSize(&width, &height);

        ComPtr<IWICFormatConverter> converter;
        wicFactory->CreateFormatConverter(&converter);
        converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

        pixels.resize(width * height * 4);
        converter->CopyPixels(nullptr, width * 4, (UINT)pixels.size(), pixels.data());
    }
    else
    {
        OutputDebugStringA("ATTENTION: Texture UI non trouvee. Utilisation d'un carre blanc par defaut.\n");
    }

    D3D12_RESOURCE_DESC texDesc = {};
    texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.DepthOrArraySize = 1;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

    m_device->GetNativeDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &texDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_uiTexture));

    UINT64 uploadBufferSize;
    m_device->GetNativeDevice()->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12_RESOURCE_DESC uploadDesc = texDesc;
    uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    uploadDesc.Width = uploadBufferSize;
    uploadDesc.Height = 1;
    uploadDesc.Format = DXGI_FORMAT_UNKNOWN;
    uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    m_device->GetNativeDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_uiTextureUpload));

    void* mappedData;
    m_uiTextureUpload->Map(0, nullptr, &mappedData);

    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
    UINT numRows; UINT64 rowSizeInBytes; UINT64 totalBytes;
    m_device->GetNativeDevice()->GetCopyableFootprints(&texDesc, 0, 1, 0, &footprint, &numRows, &rowSizeInBytes, &totalBytes);

    BYTE* pDest = reinterpret_cast<BYTE*>(mappedData);
    const BYTE* pSrc = pixels.data();
    for (UINT y = 0; y < height; ++y) {
        memcpy(pDest + footprint.Offset + y * footprint.Footprint.RowPitch, pSrc + y * width * 4, width * 4);
    }
    m_uiTextureUpload->Unmap(0, nullptr);

    m_mainContext->Reset();
    auto cmdList = m_mainContext->List();

    D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
    dstLoc.pResource = m_uiTexture.Get();
    dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dstLoc.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
    srcLoc.pResource = m_uiTextureUpload.Get();
    srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    srcLoc.PlacedFootprint = footprint;

    cmdList->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_uiTexture.Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    cmdList->ResourceBarrier(1, &barrier);

    m_mainContext->Execute(true);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += (SIZE_T)(2 * m_maxObjects + m_maxPass) * m_device->GetCbvSrvUavSize();
    m_device->GetNativeDevice()->CreateShaderResourceView(m_uiTexture.Get(), &srvDesc, handle);
}

int GraphicsEngine::Load3DTexture(const std::wstring& filepath)
{
    if (m_currentTextureCount >= m_maxTextures) return -1;

    HRESULT hr;
    ComPtr<IWICImagingFactory> wicFactory;
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

    ComPtr<IWICBitmapDecoder> decoder;
    hr = wicFactory->CreateDecoderFromFilename(filepath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

    UINT width = 1, height = 1;
    std::vector<BYTE> pixels(4, 255);

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICBitmapFrameDecode> frame;
        decoder->GetFrame(0, &frame);
        frame->GetSize(&width, &height);

        ComPtr<IWICFormatConverter> converter;
        wicFactory->CreateFormatConverter(&converter);
        converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

        pixels.resize(width * height * 4);
        converter->CopyPixels(nullptr, width * 4, (UINT)pixels.size(), pixels.data());
    }
    else
    {
        OutputDebugStringA("ATTENTION: Texture 3D non trouvee. Utilisation d'un carre blanc par defaut.\n");
    }

    D3D12_RESOURCE_DESC texDesc = {};
    texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.DepthOrArraySize = 1;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

    ComPtr<ID3D12Resource> texture;
    m_device->GetNativeDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &texDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&texture));

    UINT64 uploadBufferSize;
    m_device->GetNativeDevice()->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12_RESOURCE_DESC uploadDesc = texDesc;
    uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    uploadDesc.Width = uploadBufferSize;
    uploadDesc.Height = 1;
    uploadDesc.Format = DXGI_FORMAT_UNKNOWN;
    uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ComPtr<ID3D12Resource> textureUpload;
    m_device->GetNativeDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&textureUpload));

    void* mappedData;
    textureUpload->Map(0, nullptr, &mappedData);

    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
    UINT numRows; UINT64 rowSizeInBytes; UINT64 totalBytes;
    m_device->GetNativeDevice()->GetCopyableFootprints(&texDesc, 0, 1, 0, &footprint, &numRows, &rowSizeInBytes, &totalBytes);

    BYTE* pDest = reinterpret_cast<BYTE*>(mappedData);
    const BYTE* pSrc = pixels.data();
    for (UINT y = 0; y < height; ++y) {
        memcpy(pDest + footprint.Offset + y * footprint.Footprint.RowPitch, pSrc + y * width * 4, width * 4);
    }
    textureUpload->Unmap(0, nullptr);

    m_mainContext->Reset();
    auto cmdList = m_mainContext->List();

    D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
    dstLoc.pResource = texture.Get();
    dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dstLoc.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
    srcLoc.pResource = textureUpload.Get();
    srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    srcLoc.PlacedFootprint = footprint;

    cmdList->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = texture.Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    cmdList->ResourceBarrier(1, &barrier);

    m_mainContext->Execute(true);

    m_3dTextures.push_back(texture);
    m_3dTextureUploads.push_back(textureUpload);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += (SIZE_T)(2 * m_maxObjects + m_maxPass + 1 + m_currentTextureCount) * m_device->GetCbvSrvUavSize();
    m_device->GetNativeDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, handle);

    int indexToReturn = m_currentTextureCount;
    m_currentTextureCount++;

    return indexToReturn;
}

void GraphicsEngine::BuildUIMesh()
{
    std::vector<Vertex> vertices(4);

    vertices[0].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f); vertices[0].m_uv = XMFLOAT2(0.0f, 0.0f);
    vertices[1].m_pos = XMFLOAT3(1.0f, 0.0f, 0.0f); vertices[1].m_uv = XMFLOAT2(1.0f, 0.0f);
    vertices[2].m_pos = XMFLOAT3(0.0f, 1.0f, 0.0f); vertices[2].m_uv = XMFLOAT2(0.0f, 1.0f);
    vertices[3].m_pos = XMFLOAT3(1.0f, 1.0f, 0.0f); vertices[3].m_uv = XMFLOAT2(1.0f, 1.0f);

    std::vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };

    auto vBuffer = std::make_unique<UploadBuffer<Vertex>>(m_device->GetNativeDevice(), 4, false);
    for (int i = 0; i < 4; ++i) vBuffer->CopyData(i, vertices[i]);

    auto iBuffer = std::make_unique<UploadBuffer<uint32_t>>(m_device->GetNativeDevice(), 6, false);
    for (int i = 0; i < 6; ++i) iBuffer->CopyData(i, indices[i]);

    m_uiMesh = std::make_unique<Mesh>(std::move(vBuffer), std::move(iBuffer), 4, 6, sizeof(Vertex));

    LoadUITexture(L"../../res/Font/font2.png");
}

void GraphicsEngine::UpdateSystem(Scene* scene, float deltaTime)
{
    m_transformSystem->Update(scene, deltaTime);
    m_cameraSystem->Update(scene, deltaTime);
    m_renderSystem->Update(scene, deltaTime);
    m_physicsSystem->Update(scene, deltaTime);
    m_playerController->Update(scene, deltaTime);
    m_renderUI->Update(scene, deltaTime);

}

void GraphicsEngine::OnResize(int width, int height)
{
    m_width = width;
    m_height = height;
    m_mainContext->WaitForGpu();
    m_swapChain->Resize(width, height);

    if (m_cameraEntity)
    {
        if (auto* cam = m_cameraEntity->GetComponent<CameraComponent>())
            cam->SetAspectRatio((float)width / (float)height);
    }

    m_screenViewport.Width = (float)width;
    m_screenViewport.Height = (float)height;
    m_scissorRect = { 0, 0, width, height };

    XMMATRIX orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
    XMStoreFloat4x4(&m_orthoProj, orthoMatrix);
}

Entity* GraphicsEngine::GetCameraEntity()
{
    return m_cameraEntity;
}

GraphicsEngine::~GraphicsEngine()
{
    if (m_mainContext)
        m_mainContext->WaitForGpu();
}
