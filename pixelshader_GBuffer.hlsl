SamplerState samplerState   : SAMPLER   : register(s0);
Texture2D textureDiffuse    : TEXTURE1  : register(t0);

struct PS_INPUT
{
    float4 inPositionSS         : SV_POSITION;
    float2 inTexCoord           : TEXCOORD;
    float3 inNormalWS           : NORMALWS;
    float3 inPositionWS         : POSITIONWS;
};

struct PS_OUTPUT_GBUFFER
{
    float4 outNormalWS          : SV_TARGET0;
    float4 outDiffuseTexture    : SV_TARGET1;
    float4 outPositionWS        : SV_TARGET2;
};

PS_OUTPUT_GBUFFER ps_gbuffer(PS_INPUT psInput)
{
    PS_OUTPUT_GBUFFER psOutput;
	
    float3 normal = normalize(psInput.inNormalWS);
    
    /*Sample diffuse texture*/
    float3 diffuseTexture = textureDiffuse.Sample(samplerState, psInput.inTexCoord).rgb;
	
    /*Store all data in G-buffer*/
    psOutput.outNormalWS = float4(normal, 1.0f);
    psOutput.outDiffuseTexture = float4(diffuseTexture, 1.0f);
    psOutput.outPositionWS = float4(psInput.inPositionWS, 1.0f);
    
    return psOutput;
}