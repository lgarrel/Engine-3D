#include "pch.h"

using namespace std;
using namespace DirectX;

unique_ptr<Mesh> CreateMeshFromData(Device* device, MeshData& data)
{
    vector<Vertex> engineVertices(data.Vertices.size());

    for (size_t i = 0; i < data.Vertices.size(); ++i)
    {
        const GeomVertex& src = data.Vertices[i];
        Vertex& dest = engineVertices[i];

        dest.m_pos = src.Position;
        dest.m_normal = src.Normal;
        dest.m_uv = src.TexC;
        dest.m_color = _WHITE;
    }

    const vector<uint32_t>& indices = data.Indices32;

    auto vBuffer = std::make_unique<UploadBuffer<Vertex>>(
        device->GetNativeDevice(),
        (UINT)engineVertices.size(),
        false);

    for (size_t i = 0; i < engineVertices.size(); ++i)
        vBuffer->CopyData((int)i, engineVertices[i]);

    auto iBuffer = std::make_unique<UploadBuffer<uint32_t>>(
        device->GetNativeDevice(),
        (UINT)indices.size(),
        false);

    for (size_t i = 0; i < indices.size(); ++i)
        iBuffer->CopyData((int)i, indices[i]);

    return make_unique<Mesh>(
        std::move(vBuffer),
        std::move(iBuffer),
        (UINT)engineVertices.size(),  
        (UINT)indices.size(),          
        sizeof(Vertex)                
    );
}

unique_ptr<Mesh> ShapeFactory::CreateBox(Device* device, float width, float height, float depth)
{
    GeometryGenerator geoGen;
    MeshData data = geoGen.CreateBox(width, height, depth, 0);
    return CreateMeshFromData(device, data);
}

unique_ptr<Mesh> ShapeFactory::CreateSphere(Device* device, float radius, int sliceCount, int stackCount)
{
    GeometryGenerator geoGen;
    MeshData data = geoGen.CreateSphere(radius, (uint32_t)sliceCount, (uint32_t)stackCount);
    return CreateMeshFromData(device, data);
}

unique_ptr<Mesh> ShapeFactory::CreateCylinder(Device* device, float bottomRadius, float topRadius, float height, int sliceCount, int stackCount)
{
    GeometryGenerator geoGen;
    MeshData data = geoGen.CreateCylinder(bottomRadius, topRadius, height, (uint32_t)sliceCount, (uint32_t)stackCount);
    return CreateMeshFromData(device, data);
}