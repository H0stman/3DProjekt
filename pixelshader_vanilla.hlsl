SamplerState samp : register(s0);
Texture2D tex : register(t0);

cbuffer light : register(b0)
{
    float4 col;
    float3 lightpos;
    float pad;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = tex.Sample(samp, input.texcoord);
    float3 lightdir = normalize(lightpos + input.position.xyz);
    float diffuse = max(dot(input.normal, lightdir.xyz), 0);
    return color = mul(diffuse, textureColor);

}