cbuffer constantBuffer : register(b0)
{
	float4x4 worldMatrix;
    float4x4 worldViewProjectionMatrix; 
    float4x4 worldViewProjectionMatrixLight;
};

struct VS_INPUT
{
	float3 inPosition : POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
    float3 inTangent : TANGENT;
    float3 inBiTangent : BITANGENT;
};

struct VS_OUTPUT
{
	float4 outPositionCS : SV_POSITION;
	float2 outTexCoord : TEXCOORD;
	float3 outNormalWS : NORMALWS;
	float3 outPositionWS: POSITIONWS;
    float3 outTangentWS : TANGENTWS;
    float3 outBiTangentWS : BITANGENTWS;
    float4 outPositionLightCS : POSITIONLIGHTCS;
};

VS_OUTPUT vs_main(VS_INPUT vsInput)
{
	VS_OUTPUT vsOutput;
	
	//Calculate clip-space position.
	vsOutput.outPositionCS = mul(float4(vsInput.inPosition, 1.0f), worldViewProjectionMatrix);
	
	//Convert position, normal, tangent and bitangent to world space. 
    vsOutput.outPositionWS = mul(float4(vsInput.inPosition, 1.0f), worldMatrix).xyz;
	vsOutput.outNormalWS = normalize(mul(float4(vsInput.inNormal, 0.0f), worldMatrix).xyz);
    vsOutput.outTangentWS = normalize(mul(float4(vsInput.inTangent, 0.0f), worldMatrix).xyz);
    vsOutput.outBiTangentWS = normalize(mul(float4(vsInput.inBiTangent, 0.0f), worldMatrix).xyz);
	
	//Transform the position into light projection plane clip space
    vsOutput.outPositionLightCS = mul(float4(vsInput.inPosition, 1.0f), worldViewProjectionMatrixLight);

	//Forwarding texture coord.
	vsOutput.outTexCoord = vsInput.inTexCoord;

	return vsOutput;
}