Texture2D gUITexture : register(t0);
SamplerState gSamPointWrap : register(s0);

cbuffer cbUIObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProj;
    float4 gColor;
    float4 gUvRect;
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
    float2 TexC : TEXCOORD;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
     
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    float4 posV = mul(posW, gView);
    vout.PosH = mul(posV, gProj);
    
    float uSize = gUvRect.z - gUvRect.x;
    float vSize = gUvRect.w - gUvRect.y;
    vout.TexC = float2(gUvRect.x + vin.uv.x * uSize, gUvRect.y + vin.uv.y * vSize);
    
    vout.Color = gColor;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{ 
    float4 texColor = gUITexture.Sample(gSamPointWrap, pin.TexC);
    
    return texColor * pin.Color;
}