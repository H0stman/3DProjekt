cbuffer constantBuffer : register(b0)
{
    float4x4 worldMatrix;
    float4x4 worldViewProjectionMatrix;
    float4x4 worldViewProjectionMatrixLight;
}

struct VS_IN
{
    float3 inPosition : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inTangent : TANGENT;
    float3 inBiTangent : BITANGENT;
};

float4 vs_shadow(in VS_IN vsIn) : SV_POSITION
{
    return mul(float4(vsIn.inPosition, 1.0f), worldViewProjectionMatrixLight);
}