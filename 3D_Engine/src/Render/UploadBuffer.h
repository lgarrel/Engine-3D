#pragma once 


template<typename T>
class UploadBuffer
{

public:
    static UINT CalcConstantBufferByteSize(UINT byteSize) {
        return (byteSize + 255) & ~255;
    }

    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
        mIsConstantBuffer(isConstantBuffer)
    {
        mElementByteSize = sizeof(T);
        if (isConstantBuffer)
            mElementByteSize = CalcConstantBufferByteSize(sizeof(T));

        UINT alloc = (mElementByteSize * elementCount);

        D3D12_HEAP_PROPERTIES Properties = {};
        Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
        Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        Properties.CreationNodeMask = 1;
        Properties.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC Desc = {};
        Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        Desc.Alignment = 0;
        Desc.Width = alloc;
        Desc.Height = 1;
        Desc.DepthOrArraySize = 1;
        Desc.MipLevels = 1;
        Desc.Format = DXGI_FORMAT_UNKNOWN;
        Desc.SampleDesc.Count = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        Desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        HRESULT hr = device->CreateCommittedResource(
            &Properties,
            D3D12_HEAP_FLAG_NONE,
            &Desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mUploadBuffer));

        if (FAILED(hr)) throw std::runtime_error("Failed to create UploadBuffer");

        mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

    ~UploadBuffer() {
        if (mUploadBuffer != nullptr)
            mUploadBuffer->Unmap(0, nullptr);
        mMappedData = nullptr;
    }

    ID3D12Resource* Resource() const { return mUploadBuffer.Get(); }

    void CopyData(int elementIndex, const T& data) {
        memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
    }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
    BYTE* mMappedData = nullptr;
    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;
};