SamplerState samp : register(s0);
Texture2D tex : register(t0);

cbuffer mat : register(b0)
{
    matrix rotation;
}

cbuffer light : register(b1)
{
    float4 col;
    float3 lightpos;
    float pad;
}

/* vertex attributes go here to input to the vertex shader */
struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inTangent : TANGENT;
    float3 inBiTangent : BITANGENT;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out
{
    float4 position_clip : SV_Position; // required output of VS
    float2 uv : TEX;
    float3 normal : NOR;
    float4 posworld : world;
};


void vs_main(in VS_INPUT input, out vs_out output)
{
    output.position_clip = mul(float4(input.inPosition.xyz, 1.0f), rotation);
    output.uv = input.inTexCoord;
    output.normal = mul(float4(input.inNormal, 0), rotation).xyz;
    output.posworld = output.position_clip;
}

float4 ps_main(in vs_out input) : SV_Target
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = tex.Sample(samp, input.uv);
    float3 hej = normalize(lightpos - input.posworld.xyz);
    float diffuse = max(dot(input.normal, hej.xyz), 0);
    return color = mul(diffuse, textureColor);
}