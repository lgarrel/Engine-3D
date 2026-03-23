#include "pch.h"


void SwapChain::Initialize(Device* device, HWND hwnd, int width, int height) {
    m_parentDevice = device;
     
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = 1;  
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = BufferCount;
    sd.OutputWindow = hwnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
     
    ComPtr<IDXGISwapChain> tempSwapChain;
    m_parentDevice->GetFactory()->CreateSwapChain(m_parentDevice->GetCommandQueue(), &sd, tempSwapChain.GetAddressOf());
    tempSwapChain.As(&m_swapChain);
     
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = BufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    m_parentDevice->GetNativeDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    m_parentDevice->GetNativeDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));

    Resize(width, height);
}

void SwapChain::Resize(int width, int height) {
    auto d3dDevice = m_parentDevice->GetNativeDevice();
     
    for (int i = 0; i < BufferCount; ++i) m_swapChainBuffer[i].Reset();

    m_swapChain->ResizeBuffers(BufferCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    m_currBackBuffer = 0;
     
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (int i = 0; i < BufferCount; i++) {
        m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_swapChainBuffer[i]));
         
        D3D12_CPU_DESCRIPTOR_HANDLE currentHandle = rtvHandle;
        currentHandle.ptr += (SIZE_T)i * m_parentDevice->GetRtvSize();

        m_parentDevice->GetNativeDevice()->CreateRenderTargetView(m_swapChainBuffer[i].Get(), nullptr, currentHandle);
    }
     
    D3D12_RESOURCE_DESC depthDesc = {};
    depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthDesc.Alignment = 0;
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.DepthOrArraySize = 1;
    depthDesc.MipLevels = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear = {};
    optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

    d3dDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &depthDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &optClear,
        IID_PPV_ARGS(&m_depthStencilBuffer));

    // DSV view
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

    d3dDevice->CreateDepthStencilView(
        m_depthStencilBuffer.Get(), &dsvDesc,
        m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

}

void SwapChain::Present() {
    m_swapChain->Present(1, 0); 
    m_currBackBuffer = (m_currBackBuffer + 1) % BufferCount;
}


D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::CurrentBufferView() const {
    D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += (SIZE_T)m_currBackBuffer * m_parentDevice->GetRtvSize();
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::DepthStencilView() const
{
    return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
}
