cbuffer tranform : register(b0)
{
    float4x4 worldview;
    float4x4 view;
    float4x4 projection;
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
    float3 outNormalWS : NORMAL;
    float3 outPositionWS : POSITION;
};

VS_OUTPUT vs_main(VS_INPUT vsInput)
{
    VS_OUTPUT vsOutput;
	
	//Calculate clip-space position.
    
    vsOutput.outPositionCS = mul(float4(vsInput.inPosition, 1.0f), worldview);
    vsOutput.outPositionCS = mul(float4(vsOutput.outPositionCS), view);
    vsOutput.outPositionCS = mul(float4(vsOutput.outPositionCS), projection);
	
	//Convert position, normal, tangent and bitangent to world space. 
    //vsOutput.outPositionWS = mul(float4(vsInput.inPosition, 1.0f), worldview).xyz;
    vsOutput.outNormalWS = normalize(mul(float4(vsInput.inNormal, 0.0f), worldview).xyz);

	//Forwarding texture coord.
    //vsOutput.outTexCoord = vsInput.inTexCoord;
    //vsOutput.outPositionCS = float4(vsInput.inPosition.xyz, 1.0f);

    return vsOutput;
}