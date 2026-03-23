#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 2
#endif

#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 10
#endif

#ifndef NUM_SPOT_LIGHTS
    #define NUM_SPOT_LIGHTS 4
#endif

#include "Light_Utils.hlsl"

Texture2D gDiffuseMap : register(t0);
SamplerState gSamPointWrap : register(s0);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    Material gMaterial;
};

cbuffer cbPass : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
    float3 gEyePosW;
    float cbPad1;
    Light gLights[MaxLights];
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 Color : COLOR;
    float3 Normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float4 Color : COLOR;
    float2 uv : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosW = posW.xyz;
    float4 posV = mul(posW, gView);
    vout.PosH = mul(posV, gProj);
     
    vout.NormalW = mul(vin.Normal, (float3x3) gWorld);
    vout.Color = float4(vin.Color, 1.0f);
    vout.uv = vin.uv;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float3 normalW = normalize(pin.NormalW);
    float3 toEyeW = normalize(gEyePosW - pin.PosW);
    float3 shadowFactor = 1.0f;
    
    float4 texColor = gDiffuseMap.Sample(gSamPointWrap, pin.uv);
    
    float4 directLight = ComputeLighting(gLights, gMaterial, pin.PosW, normalW, toEyeW, shadowFactor) * texColor;
    float4 ambient = float4(0.1f, 0.1f, 0.1f, 1.0f) * gMaterial.Diffuse * texColor;
    float4 finalColor = ambient + directLight;
    
    finalColor.a = gMaterial.Diffuse.a * texColor.a;
    return finalColor;
}