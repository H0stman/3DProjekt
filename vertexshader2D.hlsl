cbuffer constantBuffer : register(b0)
{
	float4x4 orthographicMatrix;
};

struct VS_OUTPUT
{
	float4 outPositionCS : SV_POSITION;
	float2 outTexCoord : TEXCOORD;
};

struct VS_INPUT
{
	float3 inPosition : POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
};

VS_OUTPUT vs_2D_main(VS_INPUT vsin)
{
	VS_OUTPUT vsout;
    vsout.outPositionCS = mul(float4(vsin.inPosition, 1.0), orthographicMatrix);
	vsout.outTexCoord = vsin.inTexCoord;
	return vsout;
}