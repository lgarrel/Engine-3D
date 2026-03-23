#include "pch.h"
#include "RootSignature.h"

RootSignature::RootSignature()
{

}

void RootSignature::BuildRootSignature()
{

	D3D12_DESCRIPTOR_RANGE cbvTable0;
	cbvTable0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	cbvTable0.NumDescriptors = 1;
	cbvTable0.BaseShaderRegister = 0;
	cbvTable0.RegisterSpace = 0;
	cbvTable0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

 	D3D12_DESCRIPTOR_RANGE srvTable;
	srvTable.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvTable.NumDescriptors = 1;
	srvTable.BaseShaderRegister = 0; 
	srvTable.RegisterSpace = 0;
	srvTable.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE cbvTable1;
	cbvTable1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	cbvTable1.NumDescriptors = 1;
	cbvTable1.BaseShaderRegister = 1;
	cbvTable1.RegisterSpace = 0;
	cbvTable1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER slotRootParameter[3];
	slotRootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	slotRootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	slotRootParameter[0].DescriptorTable.NumDescriptorRanges = 1;
	slotRootParameter[0].DescriptorTable.pDescriptorRanges = &cbvTable0;

	slotRootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	slotRootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	slotRootParameter[1].DescriptorTable.NumDescriptorRanges = 1;
	slotRootParameter[1].DescriptorTable.pDescriptorRanges = &cbvTable1;



	slotRootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	slotRootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; 
	slotRootParameter[2].DescriptorTable.NumDescriptorRanges = 1;
	slotRootParameter[2].DescriptorTable.pDescriptorRanges = &srvTable;


	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; 
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 1;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0; 
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc;
	rootSigDesc.NumParameters = 3;
	rootSigDesc.pParameters = slotRootParameter;
	rootSigDesc.NumStaticSamplers = 1; 
	rootSigDesc.pStaticSamplers = &sampler;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> serializedRootSig;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	m_serializedRootSig = serializedRootSig;
}