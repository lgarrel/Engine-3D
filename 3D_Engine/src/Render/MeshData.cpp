#include "pch.h" 



std::vector<uint16_t>& MeshData::GetIndices16()
{
    if (mIndices16.empty())
    {
        mIndices16.resize(Indices32.size());
        for (size_t i = 0; i < Indices32.size(); ++i)
            mIndices16[i] = static_cast<uint16_t>(Indices32[i]);
    }

    return mIndices16;
}