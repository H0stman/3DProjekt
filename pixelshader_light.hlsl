Texture2D textureNormal             : register(t0);
Texture2D textureDiffuse            : register(t1);
Texture2D texturePosition           : register(t2);
Texture2D texturePositionLightCS    : register(t3);
Texture2D shadowMap                 : register(t4);

SamplerState pointSampler : register(s0);

cbuffer lightConstantBuffer : register(b0)
{
    float3 ambientColor;
    float padding1;
    float3 diffuseColor;
    float padding2;
    float3 lightPosition;
    float lightIntensity;
    float3 paddingLight;
    float ambientLightIntensity;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
    float specularIntensity;
};

cbuffer transform : register(b1)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4x4 lightWVP;
    float3 cameraPos;
};

cbuffer shadowDataConstantBuffer : register(b2)
{
    float shadowMapSize;
    float shadowBias;
    int pcfCount;
    float padding;
};

float4 ps_light(in float4 inPositionSS : SV_POSITION) : SV_TARGET
{
    int3 indices = int3(inPositionSS.xy, 0);
    
    /*Retrieve all data stored in 2D-Textures (G-buffer) based on screen position*/
    float3 inNormalWS = normalize(textureNormal.Load(indices).xyz);
    float3 diffuseTextureColor = textureDiffuse.Load(indices).xyz;
    float3 inPositionWS = texturePosition.Load(indices).xyz;
    float4 inPositionLightCS = texturePositionLightCS.Load(indices);
    
    /*SHADOWS*/
    unsigned int totalNrOfTexels = (pcfCount * 2 + 1) * (pcfCount * 2 + 1);
    float texelSize = (1.0f / shadowMapSize);
    
    /*Perspective divide to get to screeen space NDC coords*/
    float3 positionLightSS = inPositionLightCS.xyz / inPositionLightCS.w;
    
    /*NDC-coords are in [-1, 1]-range, however texture coords are in range [0, 1]. 
      Multiplication by 0.5 (-0.5 for y!) and addition by 0.5 is therefore necessary to convert into correct range.*/
    float2 shadowMapTextureCoords = float2(0.5f * positionLightSS.x + 0.5f, -0.5f * positionLightSS.y + 0.5f);
    
    /*Initialize texel samples in shadow to zero.*/
    float nrOfTexelSamplesInShadow = 0.0f;
    
    /*The ''true'' depth is extracted.*/
    float depth = positionLightSS.z;
    
    /*PCF-filtering is done depending on a certain kernel size (a x a)*/
    for (int x = -pcfCount; x < pcfCount; x++)
    {
        for (int y = pcfCount; y > -pcfCount; y--)
        {
            /*Sampling is done by an offset from current actual texel, of course taking into account the size of one texel.*/
            float2 texelCoordOffset = float2(x, y) * texelSize;
            /*An actual texel with offset from current actual texel is sampled from. To get rid of shadow acne a bias is added to the result.*/
            float depthNearestLightAsInTexture = shadowMap.Sample(pointSampler, shadowMapTextureCoords + texelCoordOffset).r + shadowBias;
            /*The ''true depth'' is compared to the sampled depth. If greater, corresponding texel is in shadow, since there must be an object in front.*/
            if (depth > depthNearestLightAsInTexture)
            {
                nrOfTexelSamplesInShadow += 1.0;
            }
        }
    }
    /*To get the final shadow factor the number of samples in shadow is divided by total samples.*/
    float percentageInShadow = nrOfTexelSamplesInShadow / totalNrOfTexels;
    float finalShadowFactor = 1.0f - percentageInShadow;
    
    /**LIGHTNING**/
    
    /***Ambient***/
    
    //Calculate ambient light:
    float3 finalAmbientColor = ambientColor * ambientLightIntensity;
    
    /***Diffuse***/
    
    //Calculate vector from surface to light
    float3 surfaceToLightVector = lightPosition - inPositionWS;
    
    //Calculate length of vector (distance to light):
    float distanceToLight = length(surfaceToLightVector);
    
    //Calculate direction to Light (normalized vector to light):
    float3 directionToLightVector = normalize(surfaceToLightVector);
    
    //Calculate the attenutation of the light, i.e. the light fall-off:
    float attenuation = 1.0f / (attenuationConstant + (attenuationLinear * distanceToLight) + (attenuationQuadratic * distanceToLight * distanceToLight));
    
    //Calculating the diffuse scalar depending on the angle at which the light strikes the vertex
    float diffuseScalar = dot(directionToLightVector, inNormalWS);
    
    /***Calculate final diffuse and specular if light reaches surface***/    
    if (diffuseScalar > 0.0f)
    {
        float3 finalDiffuseColor = diffuseColor * lightIntensity * attenuation * diffuseScalar;
        
        /*The equivalent reflected vector, around the normal of the vertex position, and normalized*/
        float3 reflectLightToVec = reflect(-directionToLightVector, inNormalWS);
    
        /*Normalized vector from vector point to camera position*/
        float3 surfaceToCamera = normalize(cameraPos - inPositionWS);
    
        /*The specular scalar constant given by the angle between the two previous vectors, raised to some power.
	    The lower the power is, the greater area of specular light, and vice versa.*/
        float specularScalar = pow(saturate(dot(reflectLightToVec, surfaceToCamera)), 30.0f);
    
        /*The final specular color, using the diffuse color as its own color, also taking into account both specular
        intensity as well as point light attenuation.*/
        float3 finalSpecularColor = (diffuseColor * lightIntensity) * specularIntensity * specularScalar * attenuation;
        
        return float4(saturate((finalAmbientColor + finalDiffuseColor + finalSpecularColor) * diffuseTextureColor * finalShadowFactor), 1.0f);
    }
    else
    {
        return float4(saturate((finalAmbientColor) * diffuseTextureColor * finalShadowFactor), 1.0f);
    }
}