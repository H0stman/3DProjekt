cbuffer transform : register(b0)
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
    float4 outPositionCS        : SV_POSITION;
    float2 outTexCoord          : TEXCOORD;
    float3 outNormalWS          : NORMALWS;
    float3 outPositionWS        : POSITIONWS;
    //float3 outTangentWS       : TANGENTWS;
    //float3 outBiTangentWS     : BITANGENTWS;
    float4 outPositionLightCS   : POSITIONLIGHTCS;
};

VS_OUTPUT vs_main(VS_INPUT vsInput)
{
    VS_OUTPUT vsOutput;
    
    vsOutput.outPositionWS = mul(float4(vsInput.inPosition, 1.0f), world).xyz;
    vsOutput.outPositionCS = mul(mul(float4(vsOutput.outPositionWS, 1.0f), view), projection);

    vsOutput.outNormalWS = normalize(mul(float4(vsInput.inNormal, 0.0f), world).xyz);

    //Transform the position into light projection plane clip space
    vsOutput.outPositionLightCS = mul(float4(vsInput.inPosition, 1.0f), lightWVP);
    
	//Forwarding texture coord.
    vsOutput.outTexCoord = vsInput.inTexCoord;

    return vsOutput;
}