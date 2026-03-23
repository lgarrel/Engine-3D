#include "pch.h" 

Mesh::Mesh(std::unique_ptr<UploadBuffer<Vertex>> vBuffer,
    std::unique_ptr<UploadBuffer<std::uint32_t>> iBuffer,
    UINT vertexCount,
    UINT indexCount,
    UINT vertexStride)
    : mVertexBuffer(std::move(vBuffer)),
    mIndexBuffer(std::move(iBuffer)),
    mVertexCount(vertexCount),
    mIndexCount(indexCount),
    mVertexByteStride(vertexStride)
{
    mVertexBufferByteSize = vertexCount * vertexStride;
    mIndexBufferByteSize = indexCount * sizeof(std::uint32_t);
}

D3D12_VERTEX_BUFFER_VIEW Mesh::GetVertexBufferView() const
{
    D3D12_VERTEX_BUFFER_VIEW vbv = {};
    vbv.BufferLocation = mVertexBuffer->Resource()->GetGPUVirtualAddress();
    vbv.StrideInBytes = mVertexByteStride;
    vbv.SizeInBytes = mVertexBufferByteSize;
    return vbv;
}

D3D12_INDEX_BUFFER_VIEW Mesh::GetIndexBufferView() const
{
    D3D12_INDEX_BUFFER_VIEW ibv = {};
    ibv.BufferLocation = mIndexBuffer->Resource()->GetGPUVirtualAddress();
    ibv.Format = DXGI_FORMAT_R32_UINT;
    ibv.SizeInBytes = mIndexBufferByteSize;
    return ibv;
}