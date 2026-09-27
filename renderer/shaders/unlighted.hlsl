#include "signature.hlsl"

struct VertexIn_Unlighted
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct VertexOut_Unlighted
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut_Unlighted VS_Unlighted(VertexIn_Unlighted vin)
{
    // Transform to homogeneous clip space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    VertexOut_Unlighted vout;
    vout.PosH = mul(posW, gViewProj);
    vout.Color = vin.Color;

    return vout;
}

float4 PS_Unlighted(VertexOut_Unlighted pin) : SV_Target
{
    return pin.Color;
}