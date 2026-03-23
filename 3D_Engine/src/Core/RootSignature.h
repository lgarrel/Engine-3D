#pragma once
class RootSignature
{
	ComPtr<ID3DBlob> m_serializedRootSig;

public:
	RootSignature();
	~RootSignature() = default;

	void BuildRootSignature();

	ID3DBlob* GetSerializeRoot() const { return m_serializedRootSig.Get(); }

};

