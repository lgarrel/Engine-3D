#pragma once

class Device;

class SwapChain {
public:
    static const int BufferCount = 2;

    void Initialize(Device* device, HWND hwnd, int width, int height);
    void Resize(int width, int height);
    void Present();

    ID3D12Resource* CurrentBuffer() const { return m_swapChainBuffer[m_currBackBuffer].Get(); }
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

private:
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_swapChainBuffer[BufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;

    int m_currBackBuffer = 0;
    Device* m_parentDevice = nullptr;
};