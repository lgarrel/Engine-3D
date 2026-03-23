#pragma once

class Device;
class ShaderUtils;

class PSOUtils
{
public:
    PSOUtils();

    Microsoft::WRL::ComPtr<ID3D12PipelineState> BuildPSO(Device* device, ShaderUtils* su);
    Microsoft::WRL::ComPtr<ID3D12PipelineState> BuildUIPSO(Device* device, ShaderUtils* su);

    D3D12_RASTERIZER_DESC DefaultRasterizer();
    D3D12_BLEND_DESC DefaultBlend();
    D3D12_BLEND_DESC UIBlend();
    D3D12_DEPTH_STENCIL_DESC DefaultDepthStencil();
    D3D12_DEPTH_STENCIL_DESC UIDepthStencil();

private:
    DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
};