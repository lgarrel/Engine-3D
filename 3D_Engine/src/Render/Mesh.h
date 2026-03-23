#pragma once  

class Mesh
{
public:
    Mesh(std::unique_ptr<UploadBuffer<Vertex>> vBuffer,
        std::unique_ptr<UploadBuffer<std::uint32_t>> iBuffer,
        UINT vertexCount,
        UINT indexCount,
        UINT vertexStride);

    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

    UINT GetIndexCount() const { return mIndexCount; }
    UINT GetVertexCount() const { return mVertexCount; }

private:
    std::unique_ptr<UploadBuffer<Vertex>> mVertexBuffer;
    std::unique_ptr<UploadBuffer<std::uint32_t>> mIndexBuffer;

    UINT mVertexCount;  
    UINT mIndexCount;
    UINT mVertexByteStride;
    UINT mVertexBufferByteSize;
    UINT mIndexBufferByteSize;
};