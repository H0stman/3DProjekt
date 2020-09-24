SamplerState samplerState   : SAMPLER   : register(s0);
Texture2D textureDiffuse    : TEXTURE1  : register(t0);
Texture2D textureNormalMap  : TEXTURE2  : register(t1);

cbuffer ModelData
{
    bool hasNormalMap;
    bool hasDisplacementMap;
};

struct PS_INPUT
{
    float4 inPositionSS         : SV_POSITION;
    float2 inTexCoord           : TEXCOORD;
    float3 inNormalWS           : NORMALWS;
    float3 inPositionWS         : POSITIONWS;
    float3 inTangentWS          : TANGENTWS;
    float3 inBiTangentWS        : BITANGENTWS;
    float4 inPositionLightCS    : POSITIONLIGHTCS;
};

struct PS_OUTPUT_GBUFFER
{
    float4 outNormalWS              : SV_TARGET0;
    float4 outDiffuseTexture        : SV_TARGET1;
    float4 outPositionWS            : SV_TARGET2;
    float4 outPositionLightCS       : SV_TARGET3;
};

PS_OUTPUT_GBUFFER ps_gbuffer(PS_INPUT psInput)
{
    PS_OUTPUT_GBUFFER psOutput;
	
    float3 normal = normalize(psInput.inNormalWS);
    if (hasNormalMap == true)
    {
        /*Model has a normal map so the tbn-matrix is created from normalized tangent, 
        bitangent and normal vectors (normalization because of lerp!)*/
        float3x3 tbn = float3x3(normalize(psInput.inTangentWS),
                                normalize(psInput.inBiTangentWS),
                                          normal);
        
        /*Sample/retrieve the normal at texel decided by the current texture coord input*/ 
        float3 sampledNormal = textureNormalMap.Sample(samplerState, psInput.inTexCoord).rgb;

        /*sampledNormal contains RGB-format (0 - 255 / 0 - 1) of the normals (because sampled from a texture), 
          hence a re-mapping to [-1, 1] is necessary*/ 
        normal = sampledNormal * 2.0f - 1.0f;
        normal.y = -normal.y;
        
        /*Finally the tbn-matrix (that is in world space!) is used to bring the sampled normals
          from tangent space to world space*/
        normal = normalize(mul(normal, tbn));
    }
    
    /*Sample diffuse texture*/
    float3 diffuseTexture = textureDiffuse.Sample(samplerState, psInput.inTexCoord).rgb;
	
    /*Store all data in G-buffer*/
    psOutput.outNormalWS = float4(normal, 1.0f);
    psOutput.outDiffuseTexture = float4(diffuseTexture, 1.0f);
    psOutput.outPositionWS = float4(psInput.inPositionWS, 1.0f);
    psOutput.outPositionLightCS = psInput.inPositionLightCS;
    
    return psOutput;
}