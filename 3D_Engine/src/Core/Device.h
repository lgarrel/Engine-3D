#pragma once

class Device {
public:
    Device();
    ~Device();

    void Initialize();

    // Accesseurs
    ID3D12Device* GetNativeDevice() const { return m_d3dDevice.Get(); }
    ID3D12CommandQueue* GetCommandQueue() const { return m_commandQueue.Get(); }
    IDXGIFactory4* GetFactory() const { return m_dxgiFactory.Get(); }
    ID3D12RootSignature* GetRootSignature() const { return m_rootSignature.Get(); }

    UINT GetRtvSize() const { return m_rtvDescriptorSize; }
    UINT GetDsvSize() const { return m_dsvDescriptorSize; }
    UINT GetCbvSrvUavSize() const { return m_cbvSrvUavDescriptorSize; }

private:
    ComPtr<IDXGIFactory4> m_dxgiFactory;
    ComPtr<ID3D12Device> m_d3dDevice;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12RootSignature> m_rootSignature;

    UINT m_rtvDescriptorSize = 0;
    UINT m_dsvDescriptorSize = 0;
    UINT m_cbvSrvUavDescriptorSize = 0;
};