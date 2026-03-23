#include "pch.h"

void CommandContext::Initialize(Device* device) {
    m_parentDevice = device;
    auto d3dDevice = m_parentDevice->GetNativeDevice();

    d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_directCmdListAlloc));
    d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_directCmdListAlloc.Get(), nullptr, IID_PPV_ARGS(&m_commandList));

    m_commandList->Close();

    d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));

    m_fenceEvent = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
    if (m_fenceEvent == nullptr) {
        throw std::runtime_error("Failed to create fence event");
    }
}

CommandContext::~CommandContext() {
    if (m_fenceEvent != nullptr) {
        CloseHandle(m_fenceEvent);
        m_fenceEvent = nullptr;
    }
}

void CommandContext::Reset() {
    m_directCmdListAlloc->Reset();
    m_commandList->Reset(m_directCmdListAlloc.Get(), nullptr);
}

void CommandContext::Execute(bool waitForGpu) {
    m_commandList->Close();
    ID3D12CommandList* lists[] = { m_commandList.Get() };
    m_parentDevice->GetCommandQueue()->ExecuteCommandLists(1, lists);

    if (waitForGpu) {
        WaitForGpu();
    }
}

void CommandContext::WaitForGpu() {
    m_currentFence++;
    m_parentDevice->GetCommandQueue()->Signal(m_fence.Get(), m_currentFence);

    if (m_fence->GetCompletedValue() < m_currentFence) {
        m_fence->SetEventOnCompletion(m_currentFence, m_fenceEvent);
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
}