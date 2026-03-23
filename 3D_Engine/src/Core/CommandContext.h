#pragma once
#include "pch.h"

class Device;

class CommandContext {
public:
    void Initialize(Device* device);
    ~CommandContext(); 

    void Reset();
    void Execute(bool waitForGpu = false);
    void WaitForGpu(); 

    ID3D12GraphicsCommandList* List() const { return m_commandList.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_directCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    UINT64 m_currentFence = 0;
    HANDLE m_fenceEvent = nullptr; 

    Device* m_parentDevice = nullptr;
};