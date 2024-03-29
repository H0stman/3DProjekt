cbuffer constantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4x4 lightVP;
    float4x4 lightWVP;
    float3 cameraPos;
};

struct VS_INPUT
{
	float3 inPosition   : POSITION;
	float2 inTexCoord   : TEXCOORD;
	float3 inNormal     : NORMAL;
    float3 inTangent    : TANGENT;
    float3 inBiTangent  : BITANGENT;
};

struct VS_OUTPUT
{
	float3 outPositionWS        : POSITIONWS;
	float2 outTexCoord          : TEXCOORD;
	float3 outNormalWS          : NORMALWS;
    float3 outTangentWS         : TANGENTWS;
    float3 outBiTangentWS       : BITANGENTWS;
};

VS_OUTPUT vs_tess_main(VS_INPUT vsInput)
{
	VS_OUTPUT vsOutput;
	
	//Convert position, normal, tangent and bitangent to world space. 
    vsOutput.outPositionWS = mul(float4(vsInput.inPosition, 1.0f), world).xyz;
    vsOutput.outNormalWS = normalize(mul(float4(vsInput.inNormal, 0.0f), world).xyz);
    vsOutput.outTangentWS = normalize(mul(float4(vsInput.inTangent, 0.0f), world).xyz);
    vsOutput.outBiTangentWS = normalize(mul(float4(vsInput.inBiTangent, 0.0f), world).xyz);
	
    
    //Forwarding texture coord.
	vsOutput.outTexCoord = vsInput.inTexCoord;

	return vsOutput;
}
