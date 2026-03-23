#pragma once
#include "pch.h"

class GeometryGenerator
{
public:
    using uint32 = std::uint32_t;


    MeshData CreateBox(float width, float height, float depth, uint32 numSubdivisions);
    MeshData CreateSphere(float radius, uint32 sliceCount, uint32 stackCount);
    MeshData CreateCylinder(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount);

private:
    void Subdivide(MeshData& meshData);
    GeomVertex MidPoint(const GeomVertex& v0, const GeomVertex& v1);
    void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
    void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
};