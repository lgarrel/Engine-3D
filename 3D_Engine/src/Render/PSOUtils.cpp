#include "pch.h"
#include "PSOUtils.h"
#include "ShaderUtils.h"

PSOUtils::PSOUtils()
{
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> PSOUtils::BuildPSO(Device* device, ShaderUtils* su)
{
    if (device == nullptr || su == nullptr) return nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = su->GetInputLayout();
    ComPtr<ID3DBlob> mvsByteCode = su->GetMVSCode();
    ComPtr<ID3DBlob> mpsByteCode = su->GetMPSCode();

    ComPtr<ID3D12RootSignature> rootSignature = device->GetRootSignature();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    psoDesc.InputLayout = { inputLayout.data(), (UINT)inputLayout.size() };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = { reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()), mvsByteCode->GetBufferSize() };
    psoDesc.PS = { reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()), mpsByteCode->GetBufferSize() };

    psoDesc.RasterizerState = DefaultRasterizer();
    psoDesc.BlendState = DefaultBlend();
    psoDesc.DepthStencilState = DefaultDepthStencil();
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = m_backBufferFormat;
    psoDesc.DSVFormat = m_depthStencilFormat;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;
    HRESULT hr = device->GetNativeDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));

    if (FAILED(hr)) {
        throw std::runtime_error("Erreur fatale : Impossible de créer le PSO.");
    }

    return pso;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> PSOUtils::BuildUIPSO(Device* device, ShaderUtils* su)
{
    if (device == nullptr || su == nullptr) return nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = su->GetInputLayout();
    ComPtr<ID3DBlob> mvsByteCode = su->GetMVSUICode();
    ComPtr<ID3DBlob> mpsByteCode = su->GetMPSUICode();

    ComPtr<ID3D12RootSignature> rootSignature = device->GetRootSignature();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    psoDesc.InputLayout = { inputLayout.data(), (UINT)inputLayout.size() };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = { reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()), mvsByteCode->GetBufferSize() };
    psoDesc.PS = { reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()), mpsByteCode->GetBufferSize() };

    psoDesc.RasterizerState = DefaultRasterizer();
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; 
    psoDesc.BlendState = UIBlend();
    psoDesc.DepthStencilState = UIDepthStencil();
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = m_backBufferFormat;
    psoDesc.DSVFormat = m_depthStencilFormat;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;
    HRESULT hr = device->GetNativeDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));

    if (FAILED(hr)) {
        throw std::runtime_error("Erreur fatale : Impossible de créer le PSO UI.");
    }

    return pso;
}

D3D12_RASTERIZER_DESC PSOUtils::DefaultRasterizer()
{
    D3D12_RASTERIZER_DESC rasterizer = {};

    rasterizer.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizer.CullMode = D3D12_CULL_MODE_BACK;
    rasterizer.FrontCounterClockwise = FALSE;
    rasterizer.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    rasterizer.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterizer.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizer.DepthClipEnable = TRUE;
    rasterizer.MultisampleEnable = FALSE;
    rasterizer.AntialiasedLineEnable = FALSE;
    rasterizer.ForcedSampleCount = 0;
    rasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    return rasterizer;
}

D3D12_BLEND_DESC PSOUtils::DefaultBlend()
{
    D3D12_BLEND_DESC blend{};

    blend.AlphaToCoverageEnable = FALSE;
    blend.IndependentBlendEnable = FALSE;
    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
    {
        FALSE,FALSE,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        blend.RenderTarget[i] = defaultRenderTargetBlendDesc;

    return blend;
}

D3D12_BLEND_DESC PSOUtils::UIBlend()
{
    D3D12_BLEND_DESC blend = DefaultBlend();
    blend.RenderTarget[0].BlendEnable = TRUE;
    blend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    blend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    blend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    blend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    blend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

    return blend;
}

D3D12_DEPTH_STENCIL_DESC PSOUtils::DefaultDepthStencil()
{
    D3D12_DEPTH_STENCIL_DESC depthStencil = {};

    depthStencil.DepthEnable = TRUE;
    depthStencil.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencil.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    depthStencil.StencilEnable = FALSE;
    depthStencil.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    depthStencil.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
    { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
    depthStencil.FrontFace = defaultStencilOp;
    depthStencil.BackFace = defaultStencilOp;

    return depthStencil;
}

D3D12_DEPTH_STENCIL_DESC PSOUtils::UIDepthStencil()
{
    D3D12_DEPTH_STENCIL_DESC depthStencil = DefaultDepthStencil();
    depthStencil.DepthEnable = FALSE;
    depthStencil.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    return depthStencil;
}