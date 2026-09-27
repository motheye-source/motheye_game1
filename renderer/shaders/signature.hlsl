cbuffer cbSolid : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbPass : register(b1)
{
    float4 gLightDiffuse;
    float3 gLightPosW;
};

cbuffer cbMaterial : register(b2)
{
    float4 gMaterialBaseColor;
};

cbuffer cbFrame : register(b3)
{
    float4x4 gView;
    float4x4 gViewProj;
    float4 gLightAmbient;
};

Texture2D gDiffuseMap : register(t0);
SamplerState gsamLinear : register(s0);