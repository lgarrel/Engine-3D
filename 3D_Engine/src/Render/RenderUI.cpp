#include "pch.h"

RenderUI::RenderUI()
{
}

void RenderUI::Initialize(CommandContext* context, Device* device, UploadBuffer<UIConstants>* uiCB,
    ID3D12DescriptorHeap* cbvHeap, ID3D12PipelineState* uiPso, Mesh* uiMesh)
{
    m_context = context;
    m_device = device;
    m_uiCB = uiCB;
    m_cbvHeap = cbvHeap;
    m_uiPso = uiPso;
    m_uiMesh = uiMesh;
}

void RenderUI::Update(Scene* currentScene, float deltaTime)
{
}

void RenderUI::Render(Scene* scene, const XMFLOAT4X4& orthoProj)
{
    if (!scene || !m_uiMesh)
        return;

    auto* cmdList = m_context->List();

    cmdList->SetPipelineState(m_uiPso);
    cmdList->SetGraphicsRootSignature(m_device->GetRootSignature());

    D3D12_GPU_DESCRIPTOR_HANDLE passCbvHandle = m_cbvHeap->GetGPUDescriptorHandleForHeapStart();
    passCbvHandle.ptr += (SIZE_T)(2 * GraphicsEngine::m_maxObjects) * m_device->GetCbvSrvUavSize();
    cmdList->SetGraphicsRootDescriptorTable(1, passCbvHandle);

    auto uiEntities = scene->GetEntitiesWithComponent<UIRendererComponent>();
    int cbIndex = 0;

    for (Entity* entity : uiEntities)
    {
        auto* uiComp = entity->GetComponent<UIRendererComponent>();

        D3D12_GPU_DESCRIPTOR_HANDLE texSrvHandle = m_cbvHeap->GetGPUDescriptorHandleForHeapStart();
        if (uiComp->textureIndex >= 0)
        {
            texSrvHandle.ptr += (SIZE_T)(2 * GraphicsEngine::m_maxObjects + GraphicsEngine::m_maxPass + 1 + uiComp->textureIndex) * m_device->GetCbvSrvUavSize();
        }
        else
        {
            texSrvHandle.ptr += (SIZE_T)(2 * GraphicsEngine::m_maxObjects + GraphicsEngine::m_maxPass) * m_device->GetCbvSrvUavSize();
        }
        cmdList->SetGraphicsRootDescriptorTable(2, texSrvHandle);

        if (uiComp->text.empty())
        {
            UIConstants cb;
            XMMATRIX matScale = XMMatrixScaling(uiComp->size.x, uiComp->size.y, 1.0f);
            XMMATRIX matTrans = XMMatrixTranslation(uiComp->position.x, uiComp->position.y, 0.5f);
            XMStoreFloat4x4(&cb.m_world, XMMatrixTranspose(matScale * matTrans));
            XMStoreFloat4x4(&cb.m_proj, XMMatrixTranspose(XMLoadFloat4x4(&orthoProj)));
            cb.Color = uiComp->color;
            cb.UvRect = uiComp->uvRect;

            m_uiCB->CopyData(cbIndex, cb);

            D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle = m_cbvHeap->GetGPUDescriptorHandleForHeapStart();
            cbvHandle.ptr += (SIZE_T)(GraphicsEngine::m_maxObjects + cbIndex) * m_device->GetCbvSrvUavSize();
            cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);

            D3D12_VERTEX_BUFFER_VIEW vbv = m_uiMesh->GetVertexBufferView();
            D3D12_INDEX_BUFFER_VIEW ibv = m_uiMesh->GetIndexBufferView();
            cmdList->IASetVertexBuffers(0, 1, &vbv);
            cmdList->IASetIndexBuffer(&ibv);
            cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            cmdList->DrawIndexedInstanced(m_uiMesh->GetIndexCount(), 1, 0, 0, 0);

            cbIndex++;
        }
        else
        {
            float currentX = uiComp->position.x;
            float currentY = uiComp->position.y;
            float charWidth = uiComp->size.x;
            float charHeight = uiComp->size.y;

            const float numCols = 16.0f;
            const float numRows = 6.0f;
            const int startAscii = 32;

            const float uvWidth = 1.0f / numCols;
            const float uvHeight = 1.0f / numRows;

            for (char c : uiComp->text)
            {
                if (c == ' ') { currentX += charWidth; continue; }

                int ascii = static_cast<int>(c) - startAscii;
                if (ascii < 0) ascii = 0;

                int col = ascii % (int)numCols;
                int row = ascii / (int)numCols;

                UIConstants cb;
                XMMATRIX matScale = XMMatrixScaling(charWidth, charHeight, 1.0f);
                XMMATRIX matTrans = XMMatrixTranslation(currentX, currentY, 0.5f);
                XMStoreFloat4x4(&cb.m_world, XMMatrixTranspose(matScale * matTrans));
                XMStoreFloat4x4(&cb.m_proj, XMMatrixTranspose(XMLoadFloat4x4(&orthoProj)));
                cb.Color = uiComp->color;

                cb.UvRect.x = col * uvWidth;
                cb.UvRect.y = row * uvHeight;
                cb.UvRect.z = cb.UvRect.x + uvWidth;
                cb.UvRect.w = cb.UvRect.y + uvHeight;

                m_uiCB->CopyData(cbIndex, cb);

                D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle = m_cbvHeap->GetGPUDescriptorHandleForHeapStart();
                cbvHandle.ptr += (SIZE_T)(GraphicsEngine::m_maxObjects + cbIndex) * m_device->GetCbvSrvUavSize();
                cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);

                D3D12_VERTEX_BUFFER_VIEW vbv = m_uiMesh->GetVertexBufferView();
                D3D12_INDEX_BUFFER_VIEW ibv = m_uiMesh->GetIndexBufferView();
                cmdList->IASetVertexBuffers(0, 1, &vbv);
                cmdList->IASetIndexBuffer(&ibv);
                cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                cmdList->DrawIndexedInstanced(m_uiMesh->GetIndexCount(), 1, 0, 0, 0);

                cbIndex++;
                currentX += charWidth;
            }
        }
    }
}
