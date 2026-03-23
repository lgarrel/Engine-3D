#pragma once
#include <d3dcompiler.h>

class ShaderUtils
{
	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	ComPtr<ID3DBlob> mvsUICode = nullptr;
	ComPtr<ID3DBlob> mpsUICode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

public:
	ShaderUtils();

	void BuildShader();
	ComPtr<ID3DBlob> CompileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint, const std::string& target);

	ComPtr<ID3DBlob> GetMVSCode() const { return mvsByteCode.Get(); }
	ComPtr<ID3DBlob> GetMPSCode() const { return mpsByteCode.Get(); }

	ComPtr<ID3DBlob> GetMVSUICode() const { return mvsUICode.Get(); }
	ComPtr<ID3DBlob> GetMPSUICode() const { return mpsUICode.Get(); }

	std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayout() const { return mInputLayout; }
};