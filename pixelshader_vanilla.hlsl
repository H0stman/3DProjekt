SamplerState samp : register(s0);
Texture2D tex : register(t0);

cbuffer light : register(b0)
{
    float4 col;
    float3 lightpos;
    float pad;
}

cbuffer tranform : register(b1)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float3 camerapos;
};

struct PS_INPUT
{
	float4 position     : SV_POSITION;
	float2 texcoord     : TEXCOORD;
	float3 normal       : NORMAL;
    float3 positionws   : POSITION;
};

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    float4 lightposWS = mul(float4(lightpos.xyz, 1), world);
    
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = tex.Sample(samp, input.texcoord);
    float3 lightdir = normalize(lightposWS.xyz - input.positionws.xyz);
    float diffuse = max(dot(input.normal, lightdir.xyz), 0);
    return mul(diffuse, textureColor);
}