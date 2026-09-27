#include "signature.hlsl"

struct VertexIn_Lighted
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 TexC : TEXCOORD;
};

struct VertexOut_Lighted
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 TexC : TEXCOORD;
};

VertexOut_Lighted VS_Lighted(VertexIn_Lighted vin)
{
    VertexOut_Lighted vout;

	// Transform to homogeneous clip space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);

    vout.PosW = posW.xyz;
    vout.PosH = mul(posW, gViewProj);

	// Since the world transform is expected to be contain uniform scaling (no shear),
	// then the normal can be trasnformed by the upper 3x3 world matrix.
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorld);
	
    vout.TexC.x = vin.TexC.x;
    vout.TexC.y = 1.0f - vin.TexC.y;
	
    return vout;
}

float4 PS_Lighted(VertexOut_Lighted pin) : SV_Target
{
	// Normalize the surface normal here, because interpolation can change length.
    float3 normal = normalize(pin.NormalW);

    float3 toLightW = normalize(gLightPosW - pin.PosW);
    float dotLN = dot(toLightW, normal);

    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamLinear, pin.TexC) * gMaterialBaseColor;

    float4 color = max(0, dotLN) * (diffuseAlbedo * gLightDiffuse);
	
    return color;
}
