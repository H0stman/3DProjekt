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
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float3 hej = normalize(lightpos - input.position.xyz);
    float diffuse = max(dot(input.normal, -hej.xyz), 0);
    //return float4(0.0f, 1.0f, 0.0f, 1.0f);
    return color = mul(diffuse, float4(0.0f, 1.0f, 0.0f, 1.0f));

}