#include "signature.hlsl"

struct VertexIn_Ambient
{
	float3 PosL		: POSITION;
	float2 TexC		: TEXCOORD;
};

struct VertexOut_Ambient
{
	float4 PosH		: SV_POSITION;
	float2 TexC		: TEXCOORD;
};

VertexOut_Ambient VS_Ambient(VertexIn_Ambient vin)
{
	VertexOut_Ambient vout;

	// Transform to homogeneous clip space.
	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(posW, gViewProj);

	vout.TexC.x = vin.TexC.x;
    vout.TexC.y = 1.0f - vin.TexC.y;

	return vout;
}

float4 PS_Ambient(VertexOut_Ambient pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gsamLinear, pin.TexC) * gMaterialBaseColor;

	return (diffuseAlbedo * gLightAmbient);
}