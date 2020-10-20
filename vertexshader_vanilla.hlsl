cbuffer tranform : register(b0)
{
    float4x4 world;
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
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNormalWS : NORMAL;
};

VS_OUTPUT vs_main(VS_INPUT vsInput)
{
    VS_OUTPUT vsOutput;
    
    vsOutput.outPosition = mul(float4(vsInput.inPosition, 1.0f), world);
    vsOutput.outPosition = mul(float4(vsOutput.outPosition), view);
    vsOutput.outPosition = mul(float4(vsOutput.outPosition), projection);

    vsOutput.outNormalWS = normalize(mul(float4(vsInput.inNormal, 0.0f), world).xyz);

	//Forwarding texture coord.
    vsOutput.outTexCoord = vsInput.inTexCoord;

    return vsOutput;
}