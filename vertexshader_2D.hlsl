cbuffer constantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4x4 lightVP;
    float4x4 lightWVP;
    float3 cameraPos;
};

struct VS_OUTPUT
{
	float4 outPositionCS	: SV_POSITION;
	float2 outTexCoord		: TEXCOORD;
};

struct VS_INPUT
{
	float3 inPosition	: POSITION;
	float2 inTexCoord	: TEXCOORD;
	float3 inNormal		: NORMAL;
	float3 inTangent	: TANGENT;
	float3 inBitangent	: BITANGENT;
};

VS_OUTPUT vs_2D_main(VS_INPUT vsin)
{
	VS_OUTPUT vsout;
    vsout.outPositionCS = mul(float4(vsin.inPosition, 1.0), projection);
	vsout.outTexCoord = vsin.inTexCoord;
	return vsout;
}