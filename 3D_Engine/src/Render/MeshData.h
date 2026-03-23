#pragma once

class MeshData
{
public:
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;

    std::vector<GeomVertex> Vertices;
    std::vector<uint32> Indices32;

    std::vector<uint16>& GetIndices16();

private:
    std::vector<uint16> mIndices16;
};